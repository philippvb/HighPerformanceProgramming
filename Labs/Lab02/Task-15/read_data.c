#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct product
{
    char name[50];
    double price;
}
product_t;

int main(int argc, char *argv[])
{   
    if (argc != 2){
    printf("Please enter the filename.\n");
    return -1;
    }
    char filename[30];
    strcpy(filename, argv[1]);
    FILE * fp = fopen(filename, "r");

    int total_prods;
    fscanf(fp, "%d", & total_prods);
    printf("Total prods: %d\n", total_prods);

    char cur_item[20];
    float cur_price;

    product_t *arr_of_prods = malloc((total_prods-1) * sizeof(product_t));
    int n_of_prods = 0;

    while (fscanf(fp, "%s %f", cur_item, &cur_price)!=EOF){
        product_t new_item;
        strcpy(new_item.name, cur_item);
        new_item.price = cur_price;
        arr_of_prods[n_of_prods] = new_item;
        n_of_prods += 1;
    }
    
    for(int i=0; i<n_of_prods; i++){
        printf("%s %1.f\n", arr_of_prods[i].name, arr_of_prods[i].price);
    }

return 0; }