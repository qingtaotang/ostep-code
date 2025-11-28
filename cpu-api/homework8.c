#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];
    pid_t pid1, pid2;
    
    // 创建管道
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    
    // 创建第一个子进程 (执行 ls 命令)
    pid1 = fork();
    if (pid1 == 0) {
        // 第一个子进程
        close(pipefd[0]);        // 关闭读端
        dup2(pipefd[1], STDOUT_FILENO);  // 标准输出重定向到管道写端
        close(pipefd[1]);        // 关闭原始写端
        
        // 执行 ls 命令，输出将通过管道发送
        execlp("ls", "ls", "-l", NULL);
        perror("execlp ls failed");
        exit(EXIT_FAILURE);
    }
    
    // 创建第二个子进程 (执行 wc 命令统计行数)
    pid2 = fork();
    if (pid2 == 0) {
        // 第二个子进程
        close(pipefd[1]);        // 关闭写端
        dup2(pipefd[0], STDIN_FILENO);   // 标准输入重定向到管道读端
        close(pipefd[0]);        // 关闭原始读端
        
        // 执行 wc 命令，从管道读取输入并统计
        execlp("wc", "wc", "-l", NULL);
        perror("execlp wc failed");
        exit(EXIT_FAILURE);
    }
    
    // 父进程
    close(pipefd[0]);  // 关闭管道两端
    close(pipefd[1]);
    
    // 等待子进程结束
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    
    printf("Parent: Pipeline execution completed.\n");
    
    return 0;
}
