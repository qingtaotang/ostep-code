#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[])
{
    printf("hello world (pid:%d)\n", (int)getpid());
    int fd = open("./p4.output", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU); //fd需要在fork前打开，否则父子进程会分别打开文件描述符
    int rc = fork();
    int x = 100;
    if (fd < 0)
    {
        perror("open failed");
        return -1;
    }
    if (rc < 0)
    {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0)
    {
        // child (new process)
	close(STDOUT_FILENO); 
        printf("hello, I am child (pid:%d)\n", (int)getpid());
        x = 101;
        printf("x is %d in child\n", x);

        // 查看当前文件偏移量
        off_t offset = lseek(fd, 0, SEEK_CUR);
        printf("Child: file offset before write: %ld\n", offset);

        char *data = "Hello, World in child!\n";
        ssize_t bytes_written = write(fd, data, strlen(data));
        offset = lseek(fd, 0, SEEK_CUR);
        printf("Child: file offset after write: %ld\n", offset);

        if (bytes_written < 0)
        {
            perror("write failed");
        }
        int wc = wait(NULL);
	printf("wc in child is %d\n",wc);
        sleep(1);
    }
    else
    {
        // parent goes down this path (original process)
        int wc = wait(NULL);
        printf("hello, I am parent of %d (wc:%d) (pid:%d)\n",
               rc, wc, (int)getpid());
        x = 102;
        printf("x is %d in parent\n", x);

        // 查看当前文件偏移量（继承自子进程）
        off_t offset = lseek(fd, 0, SEEK_CUR);
        printf("Parent: file offset before write: %ld\n", offset);

        char *data = "Hello, World in parent!\n";
        ssize_t bytes_written = write(fd, data, strlen(data));

        char *data2 = "Hello, World in parent line2!\n";
        ssize_t bytes_written2 = write(fd, data2, strlen(data2));

        offset = lseek(fd, 0, SEEK_CUR);
        printf("Parent: file offset after write: %ld\n", offset);


        if (bytes_written < 0 || bytes_written2 < 0)
        {
            perror("write failed");
        }
        close(fd);
    }
    return 0;
}
