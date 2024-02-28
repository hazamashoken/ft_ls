#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

// a wrapper function to strcmp that will convert the string to lower before comparing
int strcasecmp(const char *s1, const char *s2) {
    for (int i = 0; s1[i] != '\0' && s2[i] != '\0'; i++) {
        char c1 = s1[i];
        char c2 = s2[i];
        if (c1 >= 'A' && c1 <= 'Z') c1 += 32;
        if (c2 >= 'A' && c2 <= 'Z') c2 += 32;
        if (c1 != c2) return c1 - c2;
    }
    return 0;
}

// Struct to hold file information
struct FileInfo {
    char *name;
    struct stat stats;
};

// Function to compare two FileInfo structs based on their names
int compareFileInfo(const void *a, const void *b) {
    const struct FileInfo *fileInfoA = (const struct FileInfo *)a;
    const struct FileInfo *fileInfoB = (const struct FileInfo *)b;
    return strcasecmp(fileInfoA->name, fileInfoB->name);
}

// Function to sort an array of FileInfo structs
void sortFileInfoArray(struct FileInfo *fileInfos, int numFiles) {
    qsort(fileInfos, numFiles, sizeof(struct FileInfo), compareFileInfo);
}

// Function to print file information
void printFileInfo(struct FileInfo fileInfo) {
    printf("%s ", fileInfo.name);

    struct passwd *pw = getpwuid(fileInfo.stats.st_uid);
    if (pw != NULL)
        printf("%s ", pw->pw_name);
    else
        printf("%d ", fileInfo.stats.st_uid);

    struct group *gr = getgrgid(fileInfo.stats.st_gid);
    if (gr != NULL)
        printf("%s ", gr->gr_name);
    else
        printf("%d ", fileInfo.stats.st_gid);

    printf("%lld ", (long long)fileInfo.stats.st_size);

    char *timeString = ctime(&fileInfo.stats.st_mtime);
    timeString[strlen(timeString) - 1] = '\0'; // Removing the newline character at the end
    printf("%s ", timeString);

    printf("\n");
}

int main() {
    DIR *dir;
    struct dirent *entry;
    struct FileInfo *fileInfos = NULL;
    int numFiles = 0;

    dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    // Read directory entries and collect file information
    while ((entry = readdir(dir)) != NULL) {
        if (strncmp(entry->d_name, ".", 1) == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        // Allocate memory for new file info
        fileInfos = realloc(fileInfos, (numFiles + 1) * sizeof(struct FileInfo));
        if (fileInfos == NULL) {
            perror("realloc");
            closedir(dir);
            exit(EXIT_FAILURE);
        }

        // Copy file name
        fileInfos[numFiles].name = strdup(entry->d_name);

        // Get file stats
        if (lstat(entry->d_name, &fileInfos[numFiles].stats) == -1) {
            perror("stat");
            closedir(dir);
            exit(EXIT_FAILURE);
        }

        numFiles++;
    }

    closedir(dir);

    // Sort file info array
    sortFileInfoArray(fileInfos, numFiles);

    // Print sorted file info
    for (int i = 0; i < numFiles; i++) {
        printFileInfo(fileInfos[i]);
    }

    // Free allocated memory
    for (int i = 0; i < numFiles; i++) {
        free(fileInfos[i].name);
    }
    free(fileInfos);

    return 0;
}
