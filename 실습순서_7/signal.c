#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

volatile sig_atomic_t signalReceived = 0;

void signal_handler(int signal) {
    printf("받은 시그널: %d. 정리하고 프로그램을 종료합니다.\n", signal);
    signalReceived = 1;
}

int main() {
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        printf("시그널 핸들러 설정 중 오류 발생.\n");
        return 1;
    }

    printf("SIGINT (Ctrl+C) 시그널 대기중\n");

    while (!signalReceived) {
        sleep(1); 
    }

    printf("프로그램을 종료합니다.\n");
    return 0;
}

