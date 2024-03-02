/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getopt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <tliangso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:05:47 by tliangso          #+#    #+#             */
/*   Updated: 2024/03/02 10:59:13 by tliangso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_getopt.h"

char *g_optarg = NULL;
int g_optind = 0;

static int	write_err(char *prog_name, char *message, char current_arg)
{
	write(STDERR_FILENO, prog_name, ft_strlen(prog_name));
	write(STDERR_FILENO, message, ft_strlen(message));
	write(STDERR_FILENO, &current_arg, 1);
	write(STDERR_FILENO, "'\n", 2);

	return ('?');
}

static int	set_optarg(int argc, char *const argv[], \
	const char *current_arg, int *current_index)
{
	if (current_arg[2] != '\0')
		g_optarg = (char *)current_arg + 2;
	else
	{
		(*current_index)++;
		if (*current_index >= argc)
			return (write_err(argv[0], ": option requires an argument -- '", current_arg[1]));
		g_optarg = argv[*current_index];
	}

	return (0);
}


int	ft_getopt(int argc, char *const argv[], const char *optstring)
{
	static int	current_index = 1;
	const char	*current_arg;
	char		*optchar;
	int			err;

	g_optarg = NULL;

	if (current_index >= argc || argv[current_index][0]  != '-')
		return (-1);

	current_arg = argv[current_index];
	if (ft_strncmp(current_arg, "--", 2) == 0)
	{
		current_index++;
		return (-1);
	}

	optchar = ft_strchr(optstring, current_arg[1]);
	if (optchar == NULL)
		return (write_err(argv[0], ": invalid option -- '", current_arg[1]));

	if (optchar[1] == ':')
	{
		err = set_optarg(argc, argv, current_arg, &current_index);
		if (err)
			return (err);
	}
	current_index++;
	return (current_arg[1]);
}


// int main(int argc, char *argv[]) {
// 	int opt;

// 	while ((opt = ft_getopt(argc, argv, "ab:c:")) != -1) {
// 		switch (opt) {
// 			case 'a':
// 				printf("Option a\n");
// 				break;
// 			case 'b':
// 				printf("Option b with argument '%s'\n", optarg);
// 				break;
// 			case 'c':
// 				printf("Option c with argument '%s'\n", optarg);
// 				break;
// 			case '?':
// 				printf("Try 'ls --help' for more information.\n");
// 				return 1;
// 			default:
// 				// Unexpected case
// 				break;
// 		}
// 	}

// 	// Process non-option arguments (if any)
// 	for (int i = optind; i < argc; i++) {
// 		printf("Non-option argument: %s\n", argv[i]);
// 	}

// 	return 0;
// }