#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "getopt.h"



int getopt_long(int argc, char *const argv[], const char *optstring, const struct option *longopts, int *longindex) {
	int opt;

	// Reset longindex if it's provided
	if (longindex != NULL)
		*longindex = -1;

	// Loop through each argument
	for (int i = 1; i < argc && argv[i] != NULL; ++i) {
		// Check if the argument is an option
		if (argv[i][0] == '-' && argv[i][1] != '\0') {
			// Check if it's a long option
			if (argv[i][1] == '-' && longopts != NULL) {
				const struct option *longopt = longopts;
				// Iterate through long options
				while (longopt->name != NULL) {
					if (strncmp(argv[i] + 2, longopt->name, strlen(longopt->name) + 1) == 0) {
						// Found a matching long option
						// Update longindex if provided
						if (longindex != NULL)
							*longindex = i;
						// Check if the long option requires an argument
						if (longopt->has_arg == required_argument || longopt->has_arg == optional_argument) {
							// Check if the argument is attached to the option
							if (argv[i][strlen(longopt->name) + 2] == '=') {
								optarg = argv[i] + strlen(longopt->name) + 3;
							} else {
								if (i + 1 < argc && argv[i + 1][0] != '-') {
									optarg = argv[++i];
								} else if (longopt->has_arg == required_argument) {
									fprintf(stderr, "Option '--%s' requires an argument.\n", longopt->name);
									return '?';
								}
							}
						}
						// Return the option's value
						return longopt->val;
					}
					longopt++;
				}
				// Unknown long option
				fprintf(stderr, "Unknown option '%s'\n", argv[i]);
				return '?';
			} else {
				// Short option
				opt = argv[i][1];
				// Find the option in the optstring
				const char *opt_ptr = strchr(optstring, opt);
				if (opt_ptr == NULL) {
					// Unknown short option
					fprintf(stderr, "Unknown option '-%c'\n", opt);
					return '?';
				}
				// Update longindex if provided
				if (longindex != NULL)
					*longindex = i;
				// Check if the short option requires an argument
				if (*(opt_ptr + 1) == ':') {
					// Argument is the next argv
					if (argv[i][2] != '\0') {
						optarg = argv[i] + 2;
					} else {
						// Argument is the next argv
						if (i + 1 < argc && argv[i + 1][0] != '-') {
							optarg = argv[++i];
						} else {
							fprintf(stderr, "Option '-%c' requires an argument.\n", opt);
							return '?';
						}
					}
				}
				// Return the option's value
				return opt;
			}
		} else {
			// Not an option
			// Stop parsing options
			return -1;
		}
	}
	return -1; // No more options
}

int main(int argc, char *argv[]) {
	int opt;
	int longindex = -1;
	const struct option longopts[] = {
		{"help", no_argument, NULL, 'h'},
		{"version", no_argument, NULL, 'v'},
		{"output", required_argument, NULL, 'o'},
		{NULL, 0, NULL, 0}
	};

	while ((opt = getopt_long(argc, argv, "ho:v", longopts, &longindex)) != -1) {
		switch (opt) {
			case 'h':
				printf("Usage: %s [OPTIONS]\n", argv[0]);
				printf("Options:\n");
				printf("  -h, --help\t\tDisplay this help message\n");
				printf("  -v, --version\t\tDisplay version information\n");
				printf("  -o, --output=FILE\tSpecify output file\n");
				return 0;
			case 'v':
				printf("Version 1.0\n");
				return 0;
			case 'o':
				printf("Output file: %s\n", optarg);
				return 1;
			case '?':
				return 1;
			default:
				printf("Unknown option\n");
				return 1;
		}
	}

	return 0;
}
