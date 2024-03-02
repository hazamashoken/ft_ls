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
    list_files(path);
    return 0;
}

void list_files(const char *path) {
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;
    struct passwd *user_info;
    struct group *group_info;
    off_t total_blocks = 0;

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

        total_blocks += file_stat.st_blocks / 2;

        if (S_ISDIR(file_stat.st_mode)) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                printf("%s:\n", full_path);
                list_files(full_path);
            }
        }

        // Print file details
        printf("%s ", (S_ISDIR(file_stat.st_mode)) ? "d" : "-");
        printf("%c%c%c%c%c%c%c%c%c%c ",
            (file_stat.st_mode & S_IRUSR) ? 'r' : '-',
            (file_stat.st_mode & S_IWUSR) ? 'w' : '-',
            (file_stat.st_mode & S_IXUSR) ? 'x' : '-',
            (file_stat.st_mode & S_IRGRP) ? 'r' : '-',
            (file_stat.st_mode & S_IWGRP) ? 'w' : '-',
            (file_stat.st_mode & S_IXGRP) ? 'x' : '-',
            (file_stat.st_mode & S_IROTH) ? 'r' : '-',
            (file_stat.st_mode & S_IWOTH) ? 'w' : '-',
            (file_stat.st_mode & S_IXOTH) ? 'x' : '-',
            (file_stat.st_mode & S_ISVTX) ? 't' : '-');

        printf("%ld ", file_stat.st_nlink);

        user_info = getpwuid(file_stat.st_uid);
        if (user_info == NULL) {
            perror("getpwuid");
            exit(EXIT_FAILURE);
        }
        printf("%s ", user_info->pw_name);

        group_info = getgrgid(file_stat.st_gid);
        if (group_info == NULL) {
            perror("getgrgid");
            exit(EXIT_FAILURE);
        }
        printf("%s ", group_info->gr_name);

        printf("%ld ", file_stat.st_size);

        char time_buffer[50];
        strftime(time_buffer, sizeof(time_buffer), "%b %e %R", localtime(&file_stat.st_mtime));
        printf("%s ", time_buffer);

        printf("%s\n", entry->d_name);
    }

    printf("total %ld\n", total_blocks);
    closedir(dir);
}
