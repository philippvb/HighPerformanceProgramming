#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define FILESIZE 17
#define FILENAME "little_bin_file"

int main(){
    FILE * fp = fopen(FILENAME, "r");

    int i;
    fread(&i, sizeof(int), 1, fp);
    printf("%d\n", i);

    double d;
    fread(&d, sizeof(double), 1, fp);
    printf("%f\n", d);

    char c;
    fread(&c, sizeof(char), 1, fp);
    printf("%c\n", c);

    float f;
    fread(&f, sizeof(float), 1, fp);
    printf("%f\n", f);

    fclose(fp);
}