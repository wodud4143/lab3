#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/sem.h>

#define SHM_SIZE 1024
#define SEM_KEY 1234

union semun {
    int val;
    struct semid_ds *buf;
    ushort *array;
};

void sem_wait(int sem_id) {
    struct sembuf sem_buf;
    sem_buf.sem_num = 0;
    sem_buf.sem_op = -1;
    sem_buf.sem_flg = SEM_UNDO;
    semop(sem_id, &sem_buf, 1);
}

void sem_post(int sem_id) {
    struct sembuf sem_buf;
    sem_buf.sem_num = 0;
    sem_buf.sem_op = 1;
    sem_buf.sem_flg = SEM_UNDO;
    semop(sem_id, &sem_buf, 1);
}

int main() {
    int shm_id;
    char *shm_ptr;
    int sem_id;
    pid_t pid;

    shm_id = shmget(IPC_PRIVATE, SHM_SIZE, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    if (shm_id == -1) {
        perror("shmget");
        exit(1);
    }

    sem_id = semget(SEM_KEY, 1, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    if (sem_id == -1) {
        sem_id = semget(SEM_KEY, 1, S_IRUSR | S_IWUSR);
    } else {
        union semun sem_arg;
        sem_arg.val = 1;
        semctl(sem_id, 0, SETVAL, sem_arg);
    }

    shm_ptr = (char *)shmat(shm_id, NULL, 0);
    if (shm_ptr == (char *)-1) {
        perror("shmat");
        exit(1);
    }

    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        sem_wait(sem_id);

        FILE *source = fopen("원본.txt", "r");
        if (source == NULL) {
            perror("fopen");
            exit(1);
        }

        FILE *destination = fopen("복사.txt", "w");
        if (destination == NULL) {
            perror("fopen");
            exit(1);
        }

        char buffer[SHM_SIZE];
        size_t bytes_read;

        while ((bytes_read = fread(buffer, 1, sizeof(buffer), source)) > 0) {
            fwrite(buffer, 1, bytes_read, destination);
        }

        fclose(source);
        fclose(destination);

        sem_post(sem_id);
        shmdt(shm_ptr);
    } else {
        sem_wait(sem_id);
        sem_post(sem_id);
        shmdt(shm_ptr);
        shmctl(shm_id, IPC_RMID, NULL);
        semctl(sem_id, 0, IPC_RMID);
    }

    return 0;
}

