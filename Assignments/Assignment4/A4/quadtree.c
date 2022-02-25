#include <stdio.h>
#include <stdlib.h>
#include "quadtree.h"

#define SUBDIVISIONS 2



node_t create_node(float lx, float ux, float ly, float uy){
    coordinate_t lower = {lx, ly};
    coordinate_t upper = {ux, uy};
    node_t new_node;
    new_node.lower = lower;
    new_node.upper = upper;
    new_node.children = NULL;
    new_node.body = NULL;
    return new_node;
}

void insert_body(node_t *cur_node, body_t *new_body){
    // printf("Inserting body (%f, %f) into node (%f, %f)\n", new_body->pos.x, new_body->pos.y, cur_node->lower.x, cur_node->lower.y);
    if(cur_node->children==NULL){
        // node is empty, thus insert body
        // printf("Node has no children yet\n");
        if(cur_node->body == NULL){
            cur_node->body = new_body;
        }
        // node is not empty, split recursively
        else{
            cur_node->children = malloc(SUBDIVISIONS*SUBDIVISIONS*sizeof(node_t));
            int id = 0;
            // printf("Splitting node\n");
            float x_step = (cur_node->upper.x - cur_node->lower.x)/SUBDIVISIONS;
            float y_step = (cur_node->upper.y - cur_node->lower.y)/SUBDIVISIONS;
            for(double x_lower=cur_node->lower.x; x_lower<cur_node->upper.x; x_lower+=x_step){
                for(double y_lower=cur_node->lower.y; y_lower<cur_node->upper.y; y_lower+=y_step){
                    // printf("%d, %f, %f\n", id, x_lower, y_lower);
                    cur_node->children[id] = create_node(x_lower, x_lower+x_step, y_lower, y_lower + y_step);
                    // check where to insert body of current node
                    if(x_lower <= cur_node->body->pos.x && cur_node->body->pos.x < (x_lower + x_step) && y_lower <= cur_node->body->pos.y && cur_node->body->pos.y < (y_lower + y_step)){
                        // printf("Insert body into child\n");
                        cur_node->children[id].body = cur_node->body;
                    }
                    //  check where to insert new body
                    if(x_lower <= new_body->pos.x && new_body->pos.x <(x_lower + x_step) && y_lower <= new_body->pos.y && new_body->pos.y < (y_lower + y_step)){
                        insert_body(&cur_node->children[id], new_body);
                    }
                    id++;
                }
            }
            cur_node->body = NULL;
        }
    }
    else{
        for(int id = 0; id<SUBDIVISIONS*SUBDIVISIONS; id++){
            if(cur_node->children[id].lower.x <= new_body->pos.x && new_body->pos.x < cur_node->children[id].upper.x && cur_node->children[id].lower.y <= new_body->pos.y && new_body->pos.y < cur_node->children[id].upper.y){
                insert_body(&cur_node->children[id], new_body);
            }
        }
    }
}

void print_level(int level){
    for(int i=0; i<level; i++){
        printf("  ");
    }
}

void print_tree(node_t tree, int level){
    print_level(level);
    printf("Node: l(%f, %f); u(%f, %f) ", tree.lower.x, tree.lower.y, tree.upper.x, tree.upper.y);
    if(tree.body != NULL){
        printf("Body: (%f, %f)", tree.body->pos.x, tree.body->pos.y);
    }
    else if (tree.children != NULL)
    {
        for(int i=0; i<SUBDIVISIONS*SUBDIVISIONS; i++){
            printf("\n");
            print_tree(tree.children[i], level+1);
        }
    }
    
}



node_t create_inital(){
    node_t start;
    coordinate_t lower;
    lower.x = 0;
    lower.y = 0;
    coordinate_t upper;
    upper.x = 1;
    upper.y = 1;
    start.lower = lower;
    start.upper = upper;
    start.children = NULL;
    start.body = NULL;
    return start;
}
