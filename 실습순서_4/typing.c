#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_TEXT_LENGTH 1024

int main() {
    char textToType[] = "The only thing we have to fear is fear itself.";
    char typedText[MAX_TEXT_LENGTH];
    time_t startTime, endTime;
    double typingDuration;
    int errors = 0, charsTyped = 0, i;

    printf("아래 문장을 입력하고 엔터를 누르세요:\n%s\n", textToType);

    time(&startTime);

    fgets(typedText, MAX_TEXT_LENGTH, stdin);

    time(&endTime);

    typingDuration = difftime(endTime, startTime);

    typedText[strcspn(typedText, "\n")] = 0;

    for (i = 0; textToType[i] != '\0'; i++) {
        if (typedText[i] == '\0' || textToType[i] != typedText[i]) {
            errors++;
        }
        if (typedText[i] != '\0') {
            charsTyped++;
        }
    }

    double speed = (charsTyped / 5.0) / (typingDuration / 60.0);

    printf("소요 시간: %.2f 초\n", typingDuration);
    printf("오류: %d\n", errors);
    printf("타자 속도: %.2f 단어/분\n", speed);

    return 0;
}

