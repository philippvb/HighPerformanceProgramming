#include <stdio.h>

int main(){
    int a = 0;
    double b=10;
    char c = 'a';
    printf("Value %d, address %p, size in bytes %zu\n", a, &a, sizeof(a));
    printf("Value %f, address %p, size in bytes %zu\n", b, &b, sizeof(b));
    printf("Value %c, address %p, size in bytes %zu\n", c, &c, sizeof(c));
    return 0;
}