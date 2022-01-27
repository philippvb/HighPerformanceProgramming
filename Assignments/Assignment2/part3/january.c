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



// insert functions
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

void delete_node(node_t **head, int day){
    // int found = 0;
    node_t *node = *head;
    node_t *father_node = NULL;
    if(node->day == day){
        *head = node->next;
    }
    else{
        while(node != NULL){
            if(node->day == day){
                father_node->next = node->next;
                free(node);
                break;
            }
            father_node = node;
            node = node->next;
        }
    }
}

void deletelist(node_t **head){
    while((*head)->next != NULL){
        node_t *node = *head;
        while(node->next->next != NULL){
            node = node->next;
        }
        free(node->next);
        node->next = NULL;
    }
    free(*head);
    *head = NULL;
}



void print_list(node_t **head){
    node_t *node = *head;
    printf("day\tmin\tmax\n");
    while(node != NULL){
        printf("%d\t%f\t%f\n", node->day, node->min, node->max);
        node = node->next;
    }
}

int scanfinput(node_t **head){
    char line[50];
    char command;
    // printf("Enter a command:");
    fgets(line, 20, stdin);
    sscanf(line, "%c", &command);
    switch (command)
    {
    case 'A':{
        node_t *new_node = malloc(sizeof(node_t));
        new_node->next = NULL;
        sscanf(line, "%c %d %lf %lf", &command, &new_node->day, &new_node->min, &new_node->max);
        insert_node(head, &new_node);
        }
        break;
    case 'D':{
        int del_id;
        sscanf(line, "%c %d", &command, &del_id);
        delete_node(head, del_id);
        }
        break;
    case 'P':
        print_list(head);
        break;
    case 'Q':{
        printf("Quitting ...\n");
        deletelist(head);
        }
        return 0;
    default:
        printf("The command %c doesn't exist\n", command);
        break;
    }
return 1;
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
    while (scanfinput(head)){
        continue;
    }
    return 0;
}

