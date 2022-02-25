// #include "read_data.h"
#include "quadtree.h"
#include <stdio.h>
#include <stdlib.h>
const int body_length = 6;

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

int main(int argc, char *argv[]){
    // if(argc != 6) {
    // printf("Give 5 input args: N filename n_steps delta_t graphics\n");
    // return -1;
    // }
    int n_bodies = atoi(argv[1]);
    const char* fileName = argv[2];
    // const char* output_file = "result.gal";
    // int steps = atoi(argv[3]);
    // timestep = atof(argv[4]);
    // int use_graphics = atoi(argv[5]); // don't use since they dont work
    body_t *p;
    read_doubles_from_file(&p, fileName, n_bodies);
    node_t tree = create_inital();
    for(int i=0; i<n_bodies; i++){
    insert_body(&tree, p+i);
    }
    // for(int i=0; i<4; i++){
    // printf("h%f, %f ", tree.children[i].lower.x, tree.children[i].lower.y);
    // printf("%d\n", tree.children[i].body != NULL);
    // }
    // // printf("%f, %f\n", tree.children[0].body->pos.x, tree.children[0].body->pos.y);
    print_tree(tree, 0);
    // printf("%f", tree.children[0].children[0].body->pos.x);

    return 0;
}