#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

int main() {
   
    FILE *file = fopen("example.txt", "w");
    if (file == NULL) {
        perror("파일 생성 실패");
        return 1;
    }
    fprintf(file, "Hello, Linux!\n");
    fclose(file);
    printf("파일 생성 성공\n");

    
    if (mkdir("example_dir", 0777) == -1) {
        perror("디렉토리 생성 실패");
        return 1;
    }
    printf("디렉토리 생성 성공\n");

   
    DIR *dir = opendir(".");
    if (dir == NULL) {
        perror("디렉토리 열기 실패");
        return 1;
    }
    struct dirent *entry;
    printf("현재 디렉토리 내용:\n");
    while ((entry = readdir(dir))) {
        printf("%s\n", entry->d_name);
    }
    closedir(dir);

  

    return 0;
}

