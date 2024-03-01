#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

void listFilesRecursively(const char *basePath, int depth) {
    char path[1024];
    struct dirent *entry;
    struct stat statbuf;

    DIR *dir = opendir(basePath);
    if (!dir) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(path, sizeof(path), "%s/%s", basePath, entry->d_name);

        if (stat(path, &statbuf) == -1) {
            perror("stat");
            continue;
        }

        if (S_ISDIR(statbuf.st_mode)) {
            printf("%*s[%s]\n", depth * 4, "", entry->d_name);
            listFilesRecursively(path, depth + 1);
        } else {
            printf("%*s- %s\n", depth * 4, "", entry->d_name);
        }
    }

    closedir(dir);
}

int main() {
    printf(".\n");
    listFilesRecursively(".", 1);
    return 0;
}
