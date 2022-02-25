#include "body.h"
typedef struct node{
    struct node* children;
    body_t* body;
    coordinate_t lower;
    coordinate_t upper;
    coordinate_t center_of_mass;
    float total_mass;
} node_t;

void insert_body(node_t *cur_node, body_t *new_body);
void print_tree(node_t tree, int level);
node_t create_inital();