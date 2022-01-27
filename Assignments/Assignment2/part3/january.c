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
    // if(head == NULL){
    //     head = new_node;
    // }
    if((*head)->day > (*new_node)->day){
        (*new_node)->next = *head;
        *head = *new_node;
    }
    else{
        insert_behind(head, new_node);
    }
}

void delete_node(node_t **head, int day){
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
    printf("day\tmin\tmax\n");
    node_t *node = *head;
    while(node != NULL){
        printf("%d\t%f\t%f\n", node->day, node->min, node->max);
        node = node->next;
    }
}

int scanfinput(node_t **head){
    printf("Enter a command:");
    char command;
    scanf(" %c", &command);
    switch (command)
    {
    case 'A':{
        int n_read;
        node_t *new_node = malloc(sizeof(node_t));
        new_node->next = NULL;
        n_read = scanf(" %d %lf %lf", &new_node->day, &new_node->min, &new_node->max);
        if ((n_read == 3) && (new_node->day > 0) && (new_node->day < 32)){
            insert_node(head, &new_node);
        }
        else{
            printf("Wrong parameter format for command \"A\" valid format is [int day double double]\n");
        }
        }
        break;
    case 'D':{
        int del_id;
        int n_read;
        n_read = scanf(" %d", &del_id);
        if(n_read==1){
            delete_node(head, del_id);
        }
        else{
            printf("Wrong parameter format for command \"D\" valid format is [int]\n");
        }
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

    // clear print stream in case of too many arguments, for example P 2
    while ( getchar() != '\n' );
return 1;
}


int main(){
    node_t **head = NULL;
    node_t *first_node = malloc(sizeof(node_t));
    first_node->day = 0;
    first_node->min = 0;
    first_node->max = 10;
    first_node->next = NULL;
    // node_t *second_node = malloc(sizeof(node_t));
    // second_node->day = 5;
    // second_node->min = 0;
    // second_node->max = 20;
    // second_node->next = NULL;
    // node_t *third_node = malloc(sizeof(node_t));
    // third_node->day = -1;
    // third_node->min = 0;
    // third_node->max = 20;
    // third_node->next = NULL;
    head = &first_node;
    // insert_node(head, &second_node);
    // insert_node(head, &third_node);
    // print_list(head);
    while (scanfinput(head)){
        continue;
    }
    return 0;
}

