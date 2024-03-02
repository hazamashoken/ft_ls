#include <stdio.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

void print_files_in_columns(const char *path) {
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;

    // Open directory
    dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    // Get terminal width
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int terminal_width = w.ws_col;

    // Get maximum file name length
    int max_name_length = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (strlen(entry->d_name) > max_name_length) {
            max_name_length = strlen(entry->d_name);
        }
    }
    closedir(dir);

    // Calculate number of columns
    int col_width = max_name_length + 2; // Adjust as needed
    int cols = terminal_width / col_width;
    if (cols == 0) {
        cols = 1; // At least one column
    }

    // Open directory again for printing
    dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    // Print files in columns
    int count = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] != '.') {  // Skip hidden files
            printf("%-*s", col_width, entry->d_name);
            count++;
            if (count % cols == 0) {
                printf("\n");
            }
        }
    }

    // Print a newline if not already done
    if (count % cols != 0) {
        printf("\n");
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    const char *path = "."; // Default to current directory
    if (argc > 1) {
        path = argv[1];
    }
    print_files_in_columns(path);
    return 0;
}
