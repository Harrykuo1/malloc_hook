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

    printf("app: start free\n\n");
    free(arr[2]);
    free(arr[3]);
    return 0;
}