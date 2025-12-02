#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main()
{
    int * arr = (int *)malloc(10 * sizeof(int)); // 分配内存但不初始化
    arr[0] = 42; // 仅初始化第一个元素
    for(int i = 0; i < 5; i++)
    {
        printf("arr[%d] = %d\n", i, arr[i]); // 打印未初始化的值（未定义行为）
    }
    free(arr);

    for(int i = 0; i < 5; i++)
    {
        printf("arr[%d] = %d\n", i, arr[i]); // 访问已释放的内存（未定义行为）
    }

    free(1024); // 尝试释放一个非动态分配的地址（未定义行为）
    return 0;
}