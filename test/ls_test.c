#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#define MAX_PATH 256

void print_long_entry(const char *path, struct stat *statbuf, off_t *total_size) {
    char permissions[11] = "----------";
    char stype;
    off_t file_size = statbuf->st_size;

    // Set file type character
    if (S_ISDIR(statbuf->st_mode)) {
        stype = 'd';
    } else if (S_ISREG(statbuf->st_mode)) {
        stype = '-';
    } else if (S_ISLNK(statbuf->st_mode)) {
        stype = 'l';
    } else {
        stype = '?';
    }

    // Set permission bits
    permissions[0] = (statbuf->st_mode & S_IRUSR) ? 'r' : '-';
    permissions[1] = (statbuf->st_mode & S_IWUSR) ? 'w' : '-';
    permissions[2] = (statbuf->st_mode & S_IXUSR) ? 'x' : '-';
    permissions[3] = (statbuf->st_mode & S_IRGRP) ? 'r' : '-';
    permissions[4] = (statbuf->st_mode & S_IWGRP) ? 'w' : '-';
    permissions[5] = (statbuf->st_mode & S_IXGRP) ? 'x' : '-';
    permissions[6] = (statbuf->st_mode & S_IROTH) ? 'r' : '-';
    permissions[7] = (statbuf->st_mode & S_IWOTH) ? 'w' : '-';
    permissions[8] = (statbuf->st_mode & S_IXOTH) ? 'x' : '-';

    // Print with formatting
    printf("%c%s %3d %u %u %ld %s %s %s\n",
           stype, permissions, statbuf->st_nlink,
           statbuf->st_uid, statbuf->st_gid,
           file_size, ctime(&statbuf->st_ctime),
           ctime(&statbuf->st_mtime), path);

    // Update total size for directories
    if (S_ISDIR(statbuf->st_mode)) {
        *total_size += file_size;
    }
}

void ls_recursively(const char *path, off_t *total_size) {
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;
    char subpath[MAX_PATH];

    // Open directory
    if ((dir = opendir(path)) == NULL) {
        perror(path);
        return;
    }

    // Print directory heading with total size
    printf("\n%s:\n", path);
    printf("total %ld\n", *total_size);

    // Read entries
    while ((entry = readdir(dir)) != NULL) {
        // Skip "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Construct subpath
        snprintf(subpath, sizeof(subpath), "%s/%s", path, entry->d_name);

        // Get stat information
        if (lstat(subpath, &statbuf) == -1) {
            perror(subpath);
            continue;
        }

        // Print entry information
        print_long_entry(subpath, &statbuf, total_size);

        // Recursively list subdirectories if it's a directory and option -R is specified
        if (S_ISDIR(statbuf.st_mode) && access(subpath, X_OK) == 0) {
            ls_recursively(subpath, total_size);
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        return 1;
    }

    off_t total_size = 0;  // Initialize total size to 0

    ls_recursively(argv[1], &total_size);

    return 0;
}
