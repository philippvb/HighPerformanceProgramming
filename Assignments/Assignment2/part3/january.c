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
void insert_behind(node_t *node, node_t *new_node){
    if(node->day == new_node->day){
        printf("Day %d already exists, skipping\n", new_node->day);
        return;
    }
    else if ((node->next == NULL) || (node->next->day > new_node->day)){
        new_node->next = node->next;
        node->next = new_node;
    }
    else insert_behind(node->next, new_node);
}

void insert_node(node_t **head, node_t *new_node){
    if(*head == NULL){
        *head = new_node;
    }
    else if((*head)->day > new_node->day){
        new_node->next = *head;
        *head = new_node;
    }
    else{
        insert_behind(*head, new_node);
    }
}

void delete_node(node_t **head, int day){
    if(*head == NULL) return;
    node_t *node = *head;
    node_t *father_node = NULL;
    // check if root is desired node
    if(node->day == day){
        *head = node->next;
    }
    else{
        // go until node found
        while(node != NULL){
            // if found delete
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

void delete_list(node_t **head){
    node_t *current = *head;
    node_t *next;
    while(current != NULL){
        next = current->next;
        free(current);
        current = next;
    }
    *head = NULL; //set the head to 0 to show the rest of the code that the list is empty
}



void print_list(node_t *node){
    printf("day\tmin\tmax\n");
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
            insert_node(head, new_node);
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
        print_list(*head);
        break;
    case 'Q':{
        printf("Quitting ...\n");
        delete_list(head);
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
    node_t *head = NULL;

    while (scanfinput(&head)){
        continue;
    }
    return 0;
}

