#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

void listFilesInDirectory(const char *basePath, int depth) {
    char path[1000];
    struct dirent *entry;
    struct stat statbuf;

    DIR *dir = opendir(basePath);
    if (!dir) {
        perror("디렉토리 열기 실패");
        return;
    }

    while ((entry = readdir(dir))) {
        snprintf(path, sizeof(path), "%s/%s", basePath, entry->d_name);
        stat(path, &statbuf);

        if (S_ISDIR(statbuf.st_mode)) {
            
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                printf("%*s디렉토리: %s\n", depth * 2, "", path);
                listFilesInDirectory(path, depth + 1);
            }
        } else {
           
            printf("%*s파일: %s\n", depth * 2, "", path);
        }
    }

    closedir(dir);
}

int main() {
    const char *basePath = "."; 
    printf("디렉토리 내용: %s\n", basePath);
    listFilesInDirectory(basePath, 1);
    return 0;
}

