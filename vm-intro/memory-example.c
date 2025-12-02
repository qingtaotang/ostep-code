#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main()
{
    char *src = "hello";
    char *dst = (char *) malloc(strlen(src) + 1); // dst需要足够大以容纳src字符串
    strcpy(dst, src); 
    printf("Copied string: %s\n", dst);
    return 0;
}