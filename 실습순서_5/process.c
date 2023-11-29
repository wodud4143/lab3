#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    pid_t pid;

    pid = fork();

    if (pid == -1) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        printf("자식 프로세스: PID = %d\n", getpid());
        execlp("/bin/ls", "ls", NULL);
    } else {
        printf("부모 프로세스: PID = %d\n", getpid());
        wait(NULL);
        printf("자식 프로세스가 종료되었습니다.\n");

        FILE *fp;
        char count[1024];
        int fileCount;

        fp = popen("ls | wc -l", "r");
        if (fp == NULL) {
            perror("popen failed");
            return 1;
        }

        while (fgets(count, sizeof(count) - 1, fp) != NULL) {
            fileCount = atoi(count);
        }

        pclose(fp);

        printf("현재 디렉토리의 파일 개수: %d\n", fileCount);
    }

    return 0;
}

