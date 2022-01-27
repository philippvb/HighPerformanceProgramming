#include <stdlib.h>
#include <stdio.h>

int CmpDouble(const void* p1, const void*p2){
    double a = *(double*) p1;
    double b = *(double*) p2;
    if(a < b) return -1;
    if(a == b) return 0;
    return 1;
}

int CmpString(const void* p1, const void*p2){
    char *string1 = *(char**) p1;
    char *string2 = *(char**) p2;
    int pos = 0;

    while((string1[pos] == string2[pos]) && (string1[pos] != '\0')){
        pos++;
    }
    if(string1[pos] < string2[pos]) return -1;
    if(string1[pos] > string2[pos]) return 1;
    return 0;
}

int main(){
    double arrDouble[] = {9.3, -2.3, 1.2, -0.4, 2, 9.2, 1, 2.1, 0, -9.2};
    int arrlen = 10;
    qsort(arrDouble, arrlen, sizeof(double), CmpDouble);
    for(int i=0; i<arrlen; i++){
        printf("%.2f\t", arrDouble[i]);
    }
    printf("\n");

    char *arrStr[] = {"daa", "cbab", "bbbb", "bababa", "ccccc", "aaaa"};
    int arrstrlen = 6;
    qsort(arrStr, arrstrlen, sizeof(char *), CmpString);
    for(int i=0; i<arrstrlen; i++){
        printf("%s\t", arrStr[i]);
    }
    return 0;
}