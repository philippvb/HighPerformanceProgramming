#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PARTICLE_LENGTH 6
#define EPSILON 10e-3
#define TIMESTEP 10e-5

typedef struct coordinate
{
    double x;
    double y;
} coordinate;


typedef struct particle
{
    coordinate pos;
    const double mass;
    coordinate vel;
    const double brightness;
} particle;


int read_doubles_from_file(particle** p, const char* fileName, int* n_particles) {
  /* Open input file and determine its size. */
  FILE* input_file = fopen(fileName, "rb");

  /* Get filesize using fseek() and ftell(). */
  fseek(input_file, 0L, SEEK_END);
  size_t fileSize = ftell(input_file);
  /* Now use fseek() again to set file position back to beginning of the file. */
  fseek(input_file, 0L, SEEK_SET);
  *n_particles = fileSize/sizeof(double)/PARTICLE_LENGTH;
  *p = malloc(*n_particles * sizeof(particle));
  /* Read contents of input_file into buffer. */
  size_t noOfItemsRead = 0;
  for(int i=0; i<*n_particles; i++){
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
    printf("Pos X: %f, Pos Y; %f, Mass: %f, Vel X: %f, Vel Y: %f, Brightness: %f\n", p->pos.x, p->pos.y, p->mass, p->vel.x, p->vel.y, p->brightness);
}


double compute_square_dist(particle a, particle b){
    return pow(a.pos.x - b.pos.x, 2) + pow(a.pos.y - b.pos.y, 2) + EPSILON;
}

coordinate compute_direction(particle a, particle b, double scaling){
    coordinate dir;
    dir.x = (a.pos.x - b.pos.x)/scaling;
    dir.y = (a.pos.y - b.pos.y)/scaling;
    return dir;
}



int main(){
    char* filename = "ref_output_data/ellipse_N_00010_after200steps.gal";
    particle * p;
    int n_particles;
    read_doubles_from_file(&p, filename, &n_particles);
    const int G = 100/n_particles;
    for(int i=0; i <n_particles; i++){
        print_particle(&p[i]);
    }
    return 0;
}