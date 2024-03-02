#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>

void list_files(const char *path);

int main(int argc, char *argv[]) {
    const char *path = ".";
    if (argc > 1) {
        path = argv[1];
    }
    printf("total ");
    list_files(path);
    return 0;
}

void list_files(const char *path) {
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;

    long long total_size = 0; // Initialize total size variable

    if ((dir = opendir(path)) == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        char full_path[1024];
        sprintf(full_path, "%s/%s", path, entry->d_name);

        if (lstat(full_path, &file_stat) == -1) {
            perror("lstat");
            exit(EXIT_FAILURE);
        }

        total_size += file_stat.st_blocks; // Add block count to total size

        if (S_ISDIR(file_stat.st_mode)) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                list_files(full_path);
            }
        }
    }

    printf("%lld ", total_size / 2); // Print total size in 512-byte blocks

    closedir(dir);
}
