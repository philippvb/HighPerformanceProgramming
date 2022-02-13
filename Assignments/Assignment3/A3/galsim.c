#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PARTICLE_LENGTH 6

typedef struct particle
{
    double pos_x;
    double pos_y;
    double mass;
    double vel_x;
    double vel_y;
    double brightness;
} particle;


int read_doubles_from_file(particle** p, const char* fileName) {
  /* Open input file and determine its size. */
  FILE* input_file = fopen(fileName, "rb");

  /* Get filesize using fseek() and ftell(). */
  fseek(input_file, 0L, SEEK_END);
  size_t fileSize = ftell(input_file);
  /* Now use fseek() again to set file position back to beginning of the file. */
  fseek(input_file, 0L, SEEK_SET);
  int n_particles = fileSize/sizeof(double);
  *p = malloc(n_particles * sizeof(particle));
  printf("test");
  /* Read contents of input_file into buffer. */
  size_t noOfItemsRead = 0;
  for(int i=0; i<n_particles; i++){
      noOfItemsRead += fread(*p + i, sizeof(char), PARTICLE_LENGTH * sizeof(double), input_file);
  }
  if(noOfItemsRead != fileSize) {
    printf("read_doubles_from_file error: failed to read file contents into buffer.\n");
    return -1;
  }
  /* OK, now we have the file contents in the buffer.
     Since we are finished with the input file, we can close it now. */
  if(fclose(input_file) != 0) {
    printf("read_doubles_from_file error: error closing input file.\n");
    return -1;
  }
  return 0;
}

void print_particle(particle* p){
    printf("Pos X: %f, Pos Y; %f, Mass: %f, Vel X: %f, Vel Y: %f, Brightness: %f\n", p->pos_x, p->pos_y, p->mass, p->vel_x, p->vel_y, p->brightness);
}

int main(){
    char* filename = "ref_output_data/ellipse_N_00010_after200steps.gal";
    particle * p;
    read_doubles_from_file(&p, filename);
    for(int i=0; i <2; i++){
        print_particle(&p[i]);
    }
    return 0;
}