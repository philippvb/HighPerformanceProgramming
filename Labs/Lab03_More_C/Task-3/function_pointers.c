#include <stdio.h>

void print_int_1(int x){
    printf("Here is the number: %d\n", x);
}

void print_int_2(int x){
    printf("Wow, %d is a really impressive number!\n", x);
}



int main(){
    int x = 10;
    void (*func_pointer)(int);
    func_pointer = print_int_1;
    func_pointer(x);
    func_pointer = print_int_2;
    func_pointer(x);
}