#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "getopt.h"

int getopt(int argc, char *const argv[], const char *optstring) {
    static int currentIndex = 1;   // Index of the next argument to be processed
    const char *currentArg;        // Current argument
    char *optchar;                 // Pointer to the current option character in optstring

    // Reset optarg for each new call to getopt
    optarg = NULL;

    // Check if all arguments have been processed
    if (currentIndex >= argc || argv[currentIndex][0] != '-') {
        return -1;
    }

    // Get the current argument
    currentArg = argv[currentIndex];

    // Check if the current argument is "--" (end of options)
    if (strcmp(currentArg, "--") == 0) {
        currentIndex++;
        return -1;
    }

    // Get the current option character
    optchar = strchr(optstring, currentArg[1]);

    if (optchar == NULL) {
        fprintf(stderr, "%s: invalid option -- '%c'\n", argv[0], currentArg[1]);
        return '?';
    }

    // Check if the option requires an argument
    if (*(optchar + 1) == ':') {
        if (currentArg[2] != '\0') {
            optarg = currentArg + 2; // Argument is attached to the option (e.g., -ofile)
        } else {
            currentIndex++;
            if (currentIndex >= argc) {
                fprintf(stderr, "%s: option requires an argument -- '%c'\n", argv[0], currentArg[1]);
                return '?';
            }
            optarg = argv[currentIndex]; // Argument is the next argument in argv
        }
    }

    // Move to the next argument for the next call to getopt
    currentIndex++;

    return currentArg[1];
}

int main(int argc, char *argv[]) {
    int opt;

    while ((opt = getopt(argc, argv, "ab:c:")) != -1) {
        switch (opt) {
            case 'a':
                printf("Option a\n");
                break;
            case 'b':
                printf("Option b with argument '%s'\n", optarg);
                break;
            case 'c':
                printf("Option c with argument '%s'\n", optarg);
                break;
            case '?':
                // Invalid option or missing argument
                break;
            default:
                // Unexpected case
                break;
        }
    }

    // Process non-option arguments (if any)
    for (int i = optind; i < argc; i++) {
        printf("Non-option argument: %s\n", argv[i]);
    }

    return 0;
}
