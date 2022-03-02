// #include "read_data.h"
#include "quadtree.h"
#include <stdio.h>
#include <stdlib.h>
const int body_length = 6;
const double epsilon = 0.001;
const double threshold = 0.02;
double timestep;

int read_doubles_from_file(body_t** p, const char* fileName, int n_bodies) {
  /* Open input file and determine its size. */
  FILE* input_file = fopen(fileName, "rb");

  /* Get filesize using fseek() and ftell(). */
  fseek(input_file, 0L, SEEK_END);
  size_t fileSize = ftell(input_file);
  /* Now use fseek() again to set file position back to beginning of the file. */
  fseek(input_file, 0L, SEEK_SET);
  if (n_bodies != fileSize/sizeof(double)/body_length){
    printf("read_doubles_from_file error: size of input file '%s' does not match the given n.\n", fileName);
    printf("For n = %d the file size is expected to be (n * sizeof(double) * 6) = %lu but the actual file size is %lu.\n", n_bodies, n_bodies * sizeof(double) * body_length, fileSize);
  }
  *p = malloc(n_bodies * sizeof(body_t));
  /* Read contents of input_file into buffer. */
  size_t noOfItemsRead = 0;
  for(int i=0; i<n_bodies; i++){
      noOfItemsRead += fread(*p + i, sizeof(char), body_length * sizeof(double), input_file);
  }
  if(noOfItemsRead != fileSize) {
    //printf("read_doubles_from_file error: failed to read file contents into buffer.\n");
    return -1;
  }
  /* OK, now we have the file contents in the buffer.
     Since we are finished with the input file, we can close it now. */
  if(fclose(input_file) != 0) {
    //printf("read_doubles_from_file error: error closing input file.\n");
    return -1;
  }
  return 0;
}

int write_doubles_to_file(body_t* p, const char* fileName, int n_bodies){
    /* Open input file and determine its size. */
    FILE* output_file = fopen(fileName, "wb");
    fwrite(p, sizeof(char), body_length * sizeof(double) * n_bodies, output_file);
    fclose(output_file);
    return 0;
}

coordinate_t compute_force(body_t* body, node_t* tree, double G){
  // printf("\nGoing into ");
  // print_node_info(*tree);
  // printf("\");
  coordinate_t force;
  force.x = 0;
  force.y = 0;
  coordinate_t direction;
  double r_cube;
  // if we have just one body, calculate this force
  if(tree->body != NULL){
    // if we are our ourself, we dont need to do anything, just return null force
    if(tree->body == body) return force;
    // printf("Computing force between %f, %f and %f, %f\n", body->pos.x, body->pos.y, tree->body->pos.x, tree->body->pos.y);
    direction = substract(body->pos, tree->body->pos);
    // printf("Dir: %f, %f\n", direction.x, direction.y);
    r_cube = norm(direction) + epsilon;
    r_cube = r_cube * r_cube * r_cube;
    // printf("Rcube %f\n", r_cube);
    // printf("Mass %f\n", tree->body->mass);
    force.x = direction.x * tree->body->mass /r_cube * -G;
    force.y = direction.y * tree->body->mass /r_cube *-G;
    // printf("Resulting force is %f, %f\n", force.x, force.y);
    return force;
  }
  else if (tree->children != NULL){
    // printf("Here");
    // print_node_info(tree->children[0]);
    // printf("%d", tree->children[1].children == NULL);
    for(int i=0; i < SUBDIVISIONS*SUBDIVISIONS; i++){
      // empty node
      if((tree->children[i].children == NULL) && (tree->children[i].body == NULL)) continue;
      // we are far enough away, can treat it as one node
      if((get_width(tree->children[i]) / norm(substract(tree->children[i].center_of_mass, body->pos))) <= threshold){
        // printf("Treating as one");
        direction = substract(body->pos, tree->children[i].center_of_mass);
        r_cube = norm(direction) + epsilon;
        r_cube = r_cube * r_cube * r_cube;
        force = add(force, multiply(direction, tree->children[i].total_mass / r_cube * -G));
      }
      // we need to descent into this child
      else{
        force = add(compute_force(body, &tree->children[i], G), force);
      }
    }
  // printf("Resulting force is %f, %f\n", force.x, force.y);
  return force;
  }
  // return force;
}

void update_body(body_t* p, coordinate_t acc){
    p->vel.x += timestep * acc.x;
    p->vel.y += timestep * acc.y;
    p->pos.x += timestep * p->vel.x;
    p->pos.y += timestep * p->vel.y;
}

void step(body_t* p, coordinate_t* acc, int n_bodies, node_t* tree, double G){
    for(int i=0; i<n_bodies; i++){
        acc[i] = compute_force(&p[i], tree, G);
        // printf("Acc is: %f, %f\n", acc[i].x, acc[i].y);
    }
    // printf("\n");
    for(int i=0; i<n_bodies; i++){
        update_body(p+i, acc[i]);
    }

    // rebuild the tree
    *tree = create_inital();
    for(int i=0; i<n_bodies; i++){
      insert_body(tree, &p[i]);
    }
}





int main(int argc, char *argv[]){
    if(argc != 6) {
    printf("Give 5 input args: N filename n_steps delta_t graphics\n");
    return -1;
    }
    int n_bodies = atoi(argv[1]);
    const char* fileName = argv[2];
    const char* output_file = "result.gal";
    int steps = atoi(argv[3]);
    timestep = atof(argv[4]);
    int use_graphics = atoi(argv[5]); // don't use since they dont work
    coordinate_t *accelerations = malloc(sizeof(coordinate_t) * n_bodies);
    const double G = ((double) 100 )/n_bodies;
    body_t *p;
    read_doubles_from_file(&p, fileName, n_bodies);
    node_t tree = create_inital();
    for(int i=0; i<n_bodies; i++){
    insert_body(&tree, p+i);
    // break;
    }
    // for(int i=0; i<4; i++){
    // printf("h%f, %f ", tree.children[i].lower.x, tree.children[i].lower.y);
    // printf("%d\n", tree.children[i].body != NULL);
    // }
    // // printf("%f, %f\n", tree.children[0].body->pos.x, tree.children[0].body->pos.y);
    // print_tree(tree, 0);
    // printf("\n");
    // coordinate_t f = compute_force(&p[0], &tree, G);
    // printf("%f, %f", f.x, f.y);
    // printf("%f", tree.children[0].children[0].body->pos.x);
    for(int i=0; i<steps; i++){
      step(p, accelerations, n_bodies, &tree, G);
    }
    // for(int i=0; i<n_bodies; i++){
    // printf("%f, %f\n", p[i].pos.x, p[i].pos.y);
    // }
    write_doubles_to_file(p, output_file, n_bodies);
    free(p);
    free(accelerations);
    return 0;
}