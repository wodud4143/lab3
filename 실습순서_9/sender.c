#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define QUEUE_NAME  "/my_queue"
#define MAX_SIZE    1024
#define MSG_STOP    "exit"
#define PERMISSIONS 0660



mqd_t create_message_queue() {
    mqd_t mq;
    struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    mq = mq_open(QUEUE_NAME, O_CREAT | O_RDWR, PERMISSIONS, &attr);
    if (mq == -1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }
    return mq;
}

void close_message_queue(mqd_t mq) {
    mq_close(mq);
    mq_unlink(QUEUE_NAME);
}

int main() {
    mqd_t mq;
    char buffer[MAX_SIZE + 1]; 

    mq = create_message_queue();

    while (1) {
        printf("Send: ");
        fflush(stdout);
        memset(buffer, 0, MAX_SIZE + 1);
        fgets(buffer, MAX_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; 

        if (mq_send(mq, buffer, strlen(buffer) + 1, 0) == -1) {
            perror("mq_send");
            continue;
        }

        if (strncmp(buffer, MSG_STOP, strlen(MSG_STOP)) == 0) {
            break;
        }
    }

    close_message_queue(mq);
    return 0;
}

