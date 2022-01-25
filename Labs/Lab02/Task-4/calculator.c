#include <stdio.h>
#include <string.h>

int main(){
    printf("Add the calculation\n");
    int a, b;
    char op;
    scanf("%d%c%d", &a, &op, &b);
    printf("The result is:\n");
    switch (op){
        case '+':
        printf("%d", a+b);
        break;
        case '-':
        printf("%d", a-b);
        break;
        case '*':
        printf("%d", a*b);
        break;
        case '/':
        printf("%d", a/b);
        break;
        default:
        printf("Please enter a valid operator, you entered %c", op);
    }
    printf("\n");
    return 0;
}