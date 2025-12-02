// memory-user.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

volatile sig_atomic_t keep_running = 1;

void handle_signal(int sig) {
    keep_running = 0;
    printf("\nReceived signal %d, cleaning up...\n", sig);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <memory_in_mb> [duration_in_seconds]\n", argv[0]);
        fprintf(stderr, "Example: %s 100      # Use 100MB indefinitely\n", argv[0]);
        fprintf(stderr, "         %s 100 30   # Use 100MB for 30 seconds\n", argv[0]);
        return 1;
    }

    // 注册信号处理
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    // 解析参数
    long megabytes = atol(argv[1]);
    long duration = 0;
    if (argc >= 3) {
        duration = atol(argv[2]);
    }

    if (megabytes <= 0) {
        fprintf(stderr, "Error: Memory size must be positive\n");
        return 1;
    }

    printf("Allocating %ld MB of memory...\n", megabytes);
    
    // 计算数组大小（以long类型为单位）
    size_t bytes = megabytes * 1024 * 1024;
    size_t array_size = bytes / sizeof(long);
    
    // 分配内存
    long *array = (long *)malloc(bytes);
    if (array == NULL) {
        fprintf(stderr, "Error: Failed to allocate %ld MB of memory\n", megabytes);
        return 1;
    }
    
    printf("Memory allocated successfully at %p\n", (void *)array);
    printf("Array size: %zu elements (%zu bytes each)\n", array_size, sizeof(long));
    
    // 初始化数组（可选）
    printf("Initializing array...\n");
    for (size_t i = 0; i < array_size; i++) {
        array[i] = i;
    }
    printf("Array initialized.\n");
    
    // 设置运行时间
    time_t start_time = time(NULL);
    time_t end_time = 0;
    if (duration > 0) {
        end_time = start_time + duration;
        printf("Will run for %ld seconds (until %s)", 
               duration, ctime(&end_time));
    } else {
        printf("Running indefinitely. Press Ctrl+C to stop.\n");
    }
    
    // 主循环：持续访问内存
    unsigned long long iterations = 0;
    printf("\nStarting memory access pattern...\n");
    printf("Iteration | Time elapsed | Memory touched\n");
    printf("----------|--------------|---------------\n");
    
    while (keep_running) {
        // 顺序访问数组的每个元素
        for (size_t i = 0; i < array_size; i++) {
            array[i] += 1;  // 修改每个元素
        }
        
        iterations++;
        
        // 每10次迭代打印一次状态
        if (iterations % 10 == 0) {
            time_t current_time = time(NULL);
            long elapsed = current_time - start_time;
            
            printf("%9llu | %12lds | %ld MB\n", 
                   iterations, elapsed, megabytes);
            
            // 检查是否达到时间限制
            if (duration > 0 && current_time >= end_time) {
                printf("Time limit reached. Stopping...\n");
                break;
            }
            
            // 检查内存是否仍然有效（可选）
            if (array[0] != iterations) {
                printf("Warning: Memory corruption detected!\n");
            }
        }
        
        // 短暂休眠以避免占用100% CPU（可选）
        // usleep(1000);  // 1ms
    }
    
    // 清理
    printf("\nCleaning up...\n");
    printf("Final value at array[0]: %ld\n", array[0]);
    printf("Total iterations: %llu\n", iterations);
    
    free(array);
    printf("Memory freed. Exiting.\n");
    
    return 0;
}
