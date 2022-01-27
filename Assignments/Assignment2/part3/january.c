#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// typedefinition for linked list node
typedef struct list_node{
    int day;
    double min;
    double max;
    struct list_node *next;
} node_t;

void insert_behind(node_t **node, node_t **new_node){
    if (((*node)->next == NULL) || ((*node)->next->day > (*new_node)->day)){
        (*new_node)->next = (*node)->next;
        (*node)->next = *new_node;
    }
    else insert_behind(&(*node)->next, new_node);
}

void insert_node(node_t **head, node_t **new_node){
    if((*head)->day > (*new_node)->day){
        (*new_node)->next = *head;
        *head = *new_node;
    }
    else{
        insert_behind(head, new_node);
    }
}


void print_list(node_t **head){
    node_t *node = *head;
    while(node != NULL){
        printf("%d\t%f\t%f\n", node->day, node->min, node->max);
        node = node->next;
    }
}

int main(){
    node_t **head = NULL;
    node_t *first_node = malloc(sizeof(node_t));
    first_node->day = 0;
    first_node->min = 0;
    first_node->max = 10;
    first_node->next = NULL;
    node_t *second_node = malloc(sizeof(node_t));
    second_node->day = 5;
    second_node->min = 0;
    second_node->max = 20;
    second_node->next = NULL;
    node_t *third_node = malloc(sizeof(node_t));
    third_node->day = -1;
    third_node->min = 0;
    third_node->max = 20;
    third_node->next = NULL;
    head = &first_node;
    insert_node(head, &second_node);
    insert_node(head, &third_node);
    print_list(head);
}