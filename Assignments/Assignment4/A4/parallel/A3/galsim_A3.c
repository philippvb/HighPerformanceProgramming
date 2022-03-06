#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>

const int body_length = 6;
const double epsilon = 0.001;
double timestep;

int n_bodies;
double G;
int n_threads;
typedef struct step_args{
    int start;
    int end;
} step_args_t;

pthread_t* threads;
step_args_t* thread_data;


pthread_mutex_t lock;
pthread_cond_t mysignal;
int waiting = 0;
int state = 0;

void barrier() {
  int mystate; 
  pthread_mutex_lock (&lock);
  mystate=state;
  waiting++;
  if (waiting == n_threads) {
    waiting = 0;
    state = 1 - mystate;
    pthread_cond_broadcast(&mysignal);
  }
  while (mystate == state) {
    pthread_cond_wait(&mysignal, &lock);
  }
  pthread_mutex_unlock(&lock);
}

typedef struct coordinate
{
    double x;
    double y;
} coordinate;


typedef struct body
{
    coordinate pos;
    double mass;
    coordinate vel;
    double brightness;
} body;

body* body_list;
coordinate* accelerations;


double get_wall_seconds(){
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec/1000000;
  return seconds;
}


int write_doubles_to_file(body* p, const char* fileName, int n_bodies){
    /* Open input file and determine its size. */
    FILE* output_file = fopen(fileName, "wb");
    fwrite(p, sizeof(char), body_length * sizeof(double) * n_bodies, output_file);
    fclose(output_file);
    return 0;
}

int read_doubles_from_file(body** p, const char* fileName, int n_bodies) {
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
  *p = malloc(n_bodies * sizeof(body));
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

void print_body(body p){
    printf("Pos X: %f, Pos Y; %f, Mass: %f, Vel X: %f, Vel Y: %f, Brightness: %f\n", p.pos.x, p.pos.y, p.mass, p.vel.x, p.vel.y, p.brightness);
}


double norm(coordinate a){
    return sqrt(a.x * a.x + a.y * a.y);
}

coordinate compute_direction(body a, body b){
    coordinate dir;
    dir.x = (a.pos.x - b.pos.x);
    dir.y = (a.pos.y - b.pos.y);
    return dir;
}

coordinate compute_force(body* p, int n_bodies, int cur_body_index, double G){
    coordinate force;
    force.x = 0;
    force.y = 0;
    double r_cube;
    coordinate direction;
    body cur_body = p[cur_body_index];
    for(int j=0; j<cur_body_index; j++){
        direction = compute_direction(cur_body, p[j]);
        r_cube = norm(direction) + epsilon;
        r_cube = r_cube * r_cube * r_cube;
        force.x += direction.x * p[j].mass / r_cube;
        force.y += direction.y * p[j].mass / r_cube;
        
    }
    for(int j=cur_body_index; j<n_bodies; j++){
        direction = compute_direction(cur_body, p[j]);
        r_cube = norm(direction) + epsilon;
        r_cube = r_cube * r_cube * r_cube;
        force.x += direction.x * p[j].mass / r_cube;
        force.y += direction.y * p[j].mass / r_cube; 
    }
    force.x *=-G;
    force.y *=-G;
    return force;
}

void update_body(body* p, coordinate acc){
    p->vel.x += timestep * acc.x;
    p->vel.y += timestep * acc.y;
    p->pos.x += timestep * p->vel.x;
    p->pos.y += timestep * p->vel.y;
}


void* make_steps(void* arg){
    step_args_t* data = (step_args_t*) arg;
    for(int i=data->start; i<data->end; i++){
        accelerations[i] = compute_force(body_list, n_bodies, i, G);
    }
    barrier();
    for(int i=data->start; i<data->end; i++){
        update_body(&body_list[i], accelerations[i]);
    }
    return NULL;
}

void step(body* p, coordinate* acc, int n_bodies, double G){
    for(int i=0; i<n_threads; i++){
        pthread_create(&threads[i], NULL, make_steps, &thread_data[i]);
    }
    for(int i=0; i<n_threads; i++){
        pthread_join(threads[i], NULL);
    }
}


int main(int argc, char *argv[]){
    if(argc != 7) {
    printf("Give 6 input args: N filename n_steps delta_t N_threads graphics\n");
    return -1;
    }
    n_bodies = atoi(argv[1]);
    const char* fileName = argv[2];
    const char* output_file = "result.gal";
    int steps = atoi(argv[3]);
    timestep = atof(argv[4]);
    n_threads = atof(argv[5]);
    int use_graphics = atoi(argv[6]); // don't use since they dont work
    read_doubles_from_file(&body_list, fileName, n_bodies);

    pthread_cond_init(&mysignal, NULL);
    pthread_mutex_init(&lock, NULL);

    accelerations = malloc(sizeof(coordinate) * n_bodies);
    G = ((double) 100 )/n_bodies;
    threads = malloc(n_threads * sizeof(pthread_t));
    thread_data = malloc(n_threads * sizeof(step_args_t));
    double block_size = (double) n_bodies/ (double) n_threads;
    for(int i=0; i<n_threads; i++){
        thread_data[i].start = (int) i * block_size;
        thread_data[i].end = (int) ((i+1) * block_size);
    }
    double start = get_wall_seconds();
    for(int s=0; s<steps; s++){
        step(body_list, accelerations, n_bodies, G);
    }
    double end = get_wall_seconds();
    printf("The execution took %f seconds\n", end-start);
    write_doubles_to_file(body_list, output_file, n_bodies);

    pthread_cond_destroy(&mysignal);
    pthread_mutex_destroy(&lock);
    free(body_list);
    free(accelerations);
    free(threads);
    free(thread_data);
}
