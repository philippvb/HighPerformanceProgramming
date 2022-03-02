#include "body.h"

#define SUBDIVISIONS 2
typedef struct node{
    struct node* children;
    body_t* body;
    coordinate_t lower;
    coordinate_t upper;
    coordinate_t center_of_mass;
    double total_mass;
    int n_bodies;
} node_t;

node_t** get_levels(node_t *tree, int maxlevel, int* n);
void get_all_bodies(node_t cur_node, body_t** out_bodies, int* n);
void insert_body(node_t *cur_node, body_t *new_body);
void print_tree(node_t tree, int level);
node_t create_inital();
coordinate_t compute_direction(body_t a, body_t b);
double norm(coordinate_t a);
double get_width(node_t node);
double get_l2_distance(body_t body, node_t node);
coordinate_t substract(coordinate_t a, coordinate_t b);
coordinate_t add(coordinate_t a, coordinate_t b);
coordinate_t multiply(coordinate_t a, double b);
void print_node_info(node_t node);
coordinate_t get_center(node_t n);