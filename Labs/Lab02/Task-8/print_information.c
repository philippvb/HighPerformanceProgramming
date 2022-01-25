#include <stdio.h>

int main(){
    long a = 0;
    float b=10;
    char c;
    printf("Value %ld, address %p, size in bytes %zu\n", a, &a, sizeof(a));
    printf("Value %f, address %p, size in bytes %zu\n", b, &b, sizeof(b));
    printf("Value %c, address %p, size in bytes %zu\n", c, &c, sizeof(c));
    return 0;
}