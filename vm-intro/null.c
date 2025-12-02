#include<stdlib.h>
#include<string.h>
#include<stdio.h>

int main()
{
    char *null_ptr = NULL;
    // 尝试解除引用NULL指针（未定义行为，可能导致段错误）
    printf("Attempting to dereference NULL pointer...\n");
    char value = *null_ptr; // 这里会引发运行时错误
    printf("Value at NULL pointer: %c\n", value);
    return 0;
}