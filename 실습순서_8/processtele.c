#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int pipefds[2];
    char buffer[1024];
    const char *message_to_child = "부모에서 자식으로 보낸 메시지";
    
    if (pipe(pipefds) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        close(pipefds[1]); 
        read(pipefds[0], buffer, sizeof(buffer)); 
        printf("자식 프로세스: 받은 메시지 - %s\n", buffer);
        close(pipefds[0]); 
    } else {
        close(pipefds[0]); 
        write(pipefds[1], message_to_child, strlen(message_to_child) + 1);
        close(pipefds[1]); 
        wait(NULL); 
        printf("부모 프로세스: 메시지를 자식에게 보냈습니다.\n");
    }

    return 0;
}

