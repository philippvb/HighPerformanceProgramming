#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{   
    if (argc != 2){
    printf("Please enter the filename.\n");
    return -1;
    }

    char filename[30];
    strcpy(filename, argv[1]);
    FILE * fp = fopen(filename, "r");
    char item[20];
    float price;
    while (fscanf(fp, "%s %f", item, &price)!=EOF){
    printf("%s %.1f\n", item, price);
    }
return 0; }