// #include "read_data.h"
#include "quadtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define NODE_LEVEL 4

const int body_length = 6;
const double epsilon = 0.001;
const double threshold = 0.25;
double timestep;

double total_update = 0;
double total_tree = 0;

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

double get_wall_seconds(){
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec/1000000;
  return seconds;
}


void compute_force(body_t* body, node_t* tree, coordinate_t *body_acc){
  coordinate_t direction;
  double r_cube;
  // if we have just one body, calculate this force
  if(tree->body != NULL){
    // if we are our ourself, we dont need to do anything, just return null force
    if(tree->body == body) return;
    direction = substract(body->pos, tree->body->pos);
    r_cube = norm(direction) + epsilon;
    r_cube = r_cube * r_cube * r_cube;
    *body_acc = add(*body_acc, multiply(direction,tree->body->mass / r_cube ));
    return;
  }
  else if (tree->children != NULL){
    for(int i=0; i < SUBDIVISIONS*SUBDIVISIONS; i++){
      // empty node
      if((tree->children[i].children == NULL) && (tree->children[i].body == NULL)) continue;
      // we are far enough away, can treat it as one node
      if((get_width(tree->children[i]) / norm(substract(tree->children[i].center_of_mass, body->pos))) <= threshold){
        direction = substract(body->pos, tree->children[i].center_of_mass);
        r_cube = norm(direction) + epsilon;
        r_cube = r_cube * r_cube * r_cube;
        *body_acc = add(*body_acc, multiply(direction, tree->children[i].total_mass / r_cube));
      }
      // we need to descent into this child
      else{
        compute_force(body, &tree->children[i], body_acc);
      }
    }
  return;
  }
}

void update_body(body_t* p, coordinate_t acc){
    p->vel.x += timestep * acc.x;
    p->vel.y += timestep * acc.y;
    p->pos.x += timestep * p->vel.x;
    p->pos.y += timestep * p->vel.y;
}

void step(body_t* p, coordinate_t* acc, int n_bodies, node_t* tree, double G){
    double update_start = get_wall_seconds();

    // get all the head nodes from each level
    int nodes=0;
    node_t** node_levels = get_levels(tree, NODE_LEVEL, &nodes);
    // clear the accelerations
    for(int i=0; i<n_bodies; i++){
        acc[i].x = 0;
        acc[i].y = 0;
    }
    // update the forces
    for(int child_id=0; child_id<nodes; child_id++){
      for(int i=0; i<n_bodies; i++){
          compute_force(&p[i], node_levels[child_id], &acc[i]);
      }
    }
    // update the positions
    for(int i=0; i<n_bodies; i++){
        update_body(p+i, multiply(acc[i], -G));
    }
    free(node_levels);

    total_update += get_wall_seconds() - update_start;
    double tree_start = get_wall_seconds();
    // rebuild the tree
    *tree = create_inital();
    for(int i=0; i<n_bodies; i++){
      insert_body(tree, &p[i]);
    }
    total_tree += get_wall_seconds() - tree_start;
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
    double start = get_wall_seconds();
    node_t tree = create_inital();
    for(int i=0; i<n_bodies; i++){
    insert_body(&tree, p+i);
    }
    // print_tree(tree, 0);
    for(int i=0; i<steps; i++){
      step(p, accelerations, n_bodies, &tree, G);
    }
    double end = get_wall_seconds();
    printf("The execution took %f seconds\n", end-start);
    printf("Time spend: Tree: %f, Update: %f\n", total_tree, total_update);
    write_doubles_to_file(p, output_file, n_bodies);
    free(p);
    free(accelerations);
    return 0;
}