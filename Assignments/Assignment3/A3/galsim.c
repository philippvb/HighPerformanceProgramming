#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

const int particle_length = 6;
const double epsilon = 0.001;
double timestep;

typedef struct coordinate
{
    double x;
    double y;
} coordinate;


typedef struct particle
{
    coordinate pos;
    double mass;
    coordinate vel;
    double brightness;
} particle;


int write_doubes_to_file(particle* p, const char* fileName, int n_particles){
    /* Open input file and determine its size. */
    FILE* output_file = fopen(fileName, "wb");
    fwrite(p, sizeof(char), particle_length * sizeof(double) * n_particles, output_file);
    fclose(output_file);
    return 0;
}

int read_doubles_from_file(particle** p, const char* fileName, int* n_particles) {
  /* Open input file and determine its size. */
  FILE* input_file = fopen(fileName, "rb");

  /* Get filesize using fseek() and ftell(). */
  fseek(input_file, 0L, SEEK_END);
  size_t fileSize = ftell(input_file);
  /* Now use fseek() again to set file position back to beginning of the file. */
  fseek(input_file, 0L, SEEK_SET);
  *n_particles = fileSize/sizeof(double)/particle_length;
  *p = malloc(*n_particles * sizeof(particle));
  /* Read contents of input_file into buffer. */
  size_t noOfItemsRead = 0;
  for(int i=0; i<*n_particles; i++){
      noOfItemsRead += fread(*p + i, sizeof(char), particle_length * sizeof(double), input_file);
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

void print_particle(particle p){
    printf("Pos X: %f, Pos Y; %f, Mass: %f, Vel X: %f, Vel Y: %f, Brightness: %f\n", p.pos.x, p.pos.y, p.mass, p.vel.x, p.vel.y, p.brightness);
}


double compute_dist(particle a, particle b){
    return sqrt(pow(a.pos.x - b.pos.x, 2) + pow(a.pos.y - b.pos.y, 2));
}

coordinate compute_direction(particle a, particle b){
    coordinate dir;
    dir.x = (a.pos.x - b.pos.x);
    dir.y = (a.pos.y - b.pos.y);
    return dir;
}

coordinate compute_force(particle* p, int n_particles, int cur_particle, double G){
    coordinate force;
    force.x = 0;
    force.y = 0;
    double r;
    coordinate newforce;
    particle cur_par = p[cur_particle];
    for(int j=0; j<n_particles; j++){
        if(j != cur_particle){
        r = compute_dist(cur_par, p[j]);
        newforce = compute_direction(cur_par, p[j]);
        force.x += newforce.x * p[j].mass / pow(r + epsilon, 3);
        force.y += newforce.y * p[j].mass / pow(r + epsilon, 3);
        }
    }
    force.x *=-G*cur_par.mass;
    force.y *=-G*cur_par.mass;
    return force;
}

particle compute_particle_update(particle* p, int n_particles, int cur_particle, double G){
    particle cur_par = p[cur_particle];
    particle new_par;
    coordinate a = compute_force(p, n_particles, cur_particle, G);
    a.x /= cur_par.mass;
    a.y /= cur_par.mass;
    new_par.vel.x = cur_par.vel.x + timestep * a.x;
    new_par.vel.y = cur_par.vel.y + timestep * a.y;
    new_par.pos.x = cur_par.pos.x + timestep * new_par.vel.x;
    new_par.pos.y = cur_par.pos.y + timestep * new_par.vel.y;
    new_par.mass = cur_par.mass;
    new_par.brightness = cur_par.brightness;
    return new_par;
}

void step(particle** p, int n_particles, double G){
    particle* new_particles = malloc(n_particles * sizeof(particle));
    for(int i=0; i<n_particles; i++){
        new_particles[i] = compute_particle_update(*p, n_particles, i, G);
    }
    free(*p);
    *p = new_particles;
}


int main(int argc, char *argv[]){
    if(argc != 6) {
    printf("Give 5 input args: N filename n_steps delta_t graphics\n");
    return -1;
    }
    int N = atoi(argv[1]);
    const char* fileName = argv[2];
    int steps = atoi(argv[3]);
    timestep = atof(argv[4]);
    int use_graphics = atoi(argv[5]); // don't use since they dont work
    char input_file[100] = "input_data/";
    char output_file[100] = "output_data/";
    strcat(input_file, fileName);
    strcat(output_file, fileName);
    particle * p;
    int n_particles;
    read_doubles_from_file(&p, input_file, &n_particles);
    const int G = 100/n_particles;
    for(int s=0; s<steps; s++){
        step(&p, n_particles, G);
    }
    write_doubes_to_file(p, output_file, n_particles);
    free(p);
}
