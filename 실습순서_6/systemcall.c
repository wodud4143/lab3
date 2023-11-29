#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int my_system(const char *command) {
    pid_t pid;
    int status;

    pid = fork();

    if (pid == -1) {
        perror("fork 실패");
        return -1;
    } else if (pid == 0) {
        execl("/bin/sh", "sh", "-c", command, NULL);
        exit(EXIT_FAILURE);
    } else {
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid 실패");
            return -1;
        }
        return status;
    }
}

int main(int argc, char *argv[]) {
    char command[1024];

    if (argc < 2) {
        printf("사용: %s <명령어>\n", argv[0]);
        return 1;
    }

    sprintf(command, "%s", argv[1]);
    for (int i = 2; i < argc; i++) {
        strcat(command, " ");
        strcat(command, argv[i]);
    }

    my_system(command);

    return 0;
}

