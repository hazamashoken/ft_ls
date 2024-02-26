/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getopt_long.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <tliangso@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 22:49:07 by tliangso          #+#    #+#             */
/*   Updated: 2024/02/27 00:42:31 by tliangso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_getopt.h"

static int write_err(char *prog_name, char *message, char current_arg)
{
	write(STDERR_FILENO, prog_name, ft_strlen(prog_name));
	write(STDERR_FILENO, message, ft_strlen(message));
	write(STDERR_FILENO, &current_arg, 1);
	write(STDERR_FILENO, "'\n", 2);
	return ('?');
}

static int write_err_long(char *prog_name, char *message, char *longname)
{
	write(STDERR_FILENO, prog_name, ft_strlen(prog_name));
	write(STDERR_FILENO, message, ft_strlen(message));
	write(STDERR_FILENO, longname, ft_strlen(longname));
	write(STDERR_FILENO, "'\n", 2);
	return ('?');
}

static int write_err_arg(const char *prog_name, const char *name)
{
	write(STDERR_FILENO, prog_name, ft_strlen(prog_name));
	write(STDERR_FILENO, ": option '--", 12);
	write(STDERR_FILENO, name, ft_strlen(name));
	write(STDERR_FILENO, "' requires an argument\n", 23);
	return ('?');
}

int ft_getopt_long(int argc, char *const argv[], const char *optstring,
				   const t_option *longopts, int *longindex)
{
	int opt;
	static int i = 1;
	const t_option *longopt;
	const char *opt_ptr;
	// static int i = 1;

	g_optarg = NULL;
	if (longindex != NULL)
		*longindex = -1;
	if (i >= argc || argv[i][0] != '-')
		return (-1);

	if (argv[i][1] == '-' && longopts != NULL)
	{
		longopt = longopts;
		while (longopt->name != NULL)
		{
			if (ft_strncmp(argv[i] + 2, longopt->name, ft_strlen(longopt->name) + 1) == 0)
			{
				if (longindex != NULL)
					*longindex = i;
				if (longopt->has_arg == REQUIRED_ARG || longopt->has_arg == OPTIONAL_ARG)
				{
					if (argv[i][ft_strlen(longopt->name) + 2] == '=')
						g_optarg = argv[i] + ft_strlen(longopt->name) + 3;
					else
					{
						if (i + 1 < argc && argv[i + 1][0] != '-')
							g_optarg = argv[++i];
						else if (longopt->has_arg == REQUIRED_ARG)
							return (write_err_arg(argv[0], longopt->name));
					}
				}
				++i;
				return (longopt->val);
			}
			longopt++;
		}
		return (write_err_long(argv[0], ": invalid option --1 '", argv[i]));
	}
	else
	{
		opt = argv[i][1];
		opt_ptr = ft_strchr(optstring, opt);
		if (opt_ptr == NULL)
			return (write_err(argv[0], ": invalid option -- '", argv[1][1]));
		if (longindex != NULL)
			*longindex = i;
		if (*(opt_ptr + 1) == ':')
		{
			if (argv[i][2] != '\0')
				g_optarg = argv[++i];
			else
			{
				if (i + 1 < argc && argv[i + 1][0] != '-')
					g_optarg = argv[++i];
				else
					return (write_err(argv[0], ": option requires an argument -- '", argv[1][1]));
			}
		}
		++i;
		return (opt);
	}
	++i;
	return (-1);
}

int main(int argc, char *argv[])
{
	int opt;
	// int longindex = -1;
	const t_option longopts[] = {
		{"help", NO_ARG, NULL, 'h'},
		{"version", NO_ARG, NULL, 'v'},
		{"output", REQUIRED_ARG, NULL, 'o'},
		{"input", REQUIRED_ARG, NULL, 'i'},
		{NULL, 0, NULL, 0}};

	while ((opt = ft_getopt_long(argc, argv, "ho:i:v", longopts, NULL)) != -1)
	{
		switch (opt)
		{
		case 'h':
			printf("Usage: %s [OPTIONS]\n", argv[0]);
			printf("Options:\n");
			printf("  -h, --help\t\tDisplay this help message\n");
			printf("  -v, --version\t\tDisplay version information\n");
			printf("  -o, --output=FILE\tSpecify output file\n");
			printf("  -i, --input=FILE\tSpecify input file\n");
			return 0;
		case 'v':
			printf("Version 1.0\n");
			return 0;
		case 'o':
			printf("Output file: %s\n", g_optarg);
			break;
		case 'i':
			printf("input file: %s\n", g_optarg);
			break;
		case '?':
			return 1;
		default:
			printf("Unknown option\n");
			return 1;
		}
	}

	return opt;
}
