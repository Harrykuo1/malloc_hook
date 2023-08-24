#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    printf("app: main func addr:%p\n\n", main);
    printf("app: start malloc\n\n");
    char *arr[10000];
    for(int i=1;i<=5;i++){
        arr[i] = malloc(100);
        memcpy(arr[i], "abcdefghijk", sizeof("abcdefghijk"));
        printf("%s\n", arr[i]);
    }
    // arr[6] = malloc(100000);
    // arr[7] = calloc(10, 10);
    // arr[1] = realloc(arr[1], 1000);

    printf("app: start free\n\n");
    free(arr[2]);
    free(arr[3]);
    return 0;
}