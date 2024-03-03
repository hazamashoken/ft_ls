#include "ft_getopt.h"

char *g_optarg = NULL;

int g_optind = 1;

static int g_inited_getopt = 0;

static char *g_nextchar;

static enum {
	PERMUTE,
	RETURN_IN_ORDER
} e_ordering;

static int g_first_nonopt;
static int g_last_nonopt;

static void exchange(char **argv)
{
	int bottom = g_first_nonopt;
	int middle = g_last_nonopt;
	int top = g_optind;
	char *tem;
	int len;

	while (top > middle && middle > bottom)
	{
		if (top - middle > middle - bottom)
		{
			len = middle - bottom;
			for (int i = 0; i < len; i++)
			{
				tem = argv[bottom + i];
				argv[bottom + i] = argv[top - (middle - bottom) + i];
				argv[top - (middle - bottom) + i] = tem;
			}
			top -= len;
		}
		else
		{
			len = top - middle;
			for (int i = 0; i < len; i++)
			{
				tem = argv[bottom + i];
				argv[bottom + i] = argv[middle + i];
				argv[middle + i] = tem;
			}
			bottom += len;
		}
	}

	g_first_nonopt += (g_optind - g_last_nonopt);
	g_last_nonopt = g_optind;
}

static const char *init_getopt(const char *optstring)
{
	g_first_nonopt = g_last_nonopt = g_optind;
	g_nextchar = NULL;

	if (optstring[0] == '-')
	{
		e_ordering = RETURN_IN_ORDER;
		++optstring;
	}
	else
		e_ordering = PERMUTE;

	return optstring;
}

static int getopt_internal(int argc, char *const *argv, const char *optstring, const t_option *longopts, int *longind, int long_only)
{
	char *nameend;
	const t_option *p;
	const t_option *pfound = NULL;
	int exact = 0;
	int ambig = 0;
	int indfound = 0;
	int option_index;

	g_optarg = NULL;
	if (g_optind == 0 || !g_inited_getopt)
	{
		if (g_optind == 0)
			g_optind = 1;
		optstring = init_getopt(optstring);
		g_inited_getopt = 1;
	}
	if (g_nextchar == NULL || *g_nextchar == '\0')
	{
		if (g_last_nonopt > g_optind)
			g_last_nonopt = g_optind;
		if (g_first_nonopt > g_optind)
			g_first_nonopt = g_optind;
		if (e_ordering == PERMUTE)
		{
			if (g_first_nonopt != g_last_nonopt && g_last_nonopt != g_optind)
				exchange((char **)argv);
			else if (g_last_nonopt != g_optind)
				g_first_nonopt = g_optind;
			while (g_optind < argc && (argv[g_optind][0] != '-' || argv[g_optind][1] == '\0'))
				g_optind++;
			g_last_nonopt = g_optind;
		}
		if (g_optind != argc && !ft_strcmp(argv[g_optind], "--"))
		{
			g_optind++;
			if (g_first_nonopt != g_last_nonopt && g_last_nonopt != g_optind)
				exchange((char **)argv);
			else if (g_first_nonopt == g_last_nonopt)
				g_first_nonopt = g_optind;
			g_last_nonopt = argc;
			g_optind = argc;
		}
		if (g_optind == argc)
		{
			if (g_first_nonopt != g_last_nonopt)
				g_optind = g_first_nonopt;
			return -1;
		}
		if (argv[g_optind][0] != '-' || argv[g_optind][1] == '\0')
		{
			g_optarg = argv[g_optind++];
			return 1;
		}
		g_nextchar = (argv[g_optind] + 1 + (longopts != NULL && argv[g_optind][1] == '-'));
	}
	if (longopts != NULL && (argv[g_optind][1] == '-' || (long_only && (argv[g_optind][2] || !ft_strchr(optstring, argv[g_optind][1])))))
	{
		for (nameend = g_nextchar; *nameend && *nameend != '='; nameend++)
			;
		for (p = longopts, option_index = 0; p->name; p++, option_index++)
			if (!ft_strncmp(p->name, g_nextchar, nameend - g_nextchar))
			{
				if ((unsigned int)(nameend - g_nextchar) == (unsigned int)ft_strlen(p->name))
				{
					pfound = p;
					indfound = option_index;
					exact = 1;
					break;
				}
				else if (pfound == NULL)
				{
					pfound = p;
					indfound = option_index;
				}
				else
					ambig = 1;
			}
		if (ambig && !exact)
		{
			ft_dprintf(STDERR_FILENO, "%s: option `%s' is ambiguous\n", argv[0], argv[g_optind]);
			g_nextchar += ft_strlen(g_nextchar);
			g_optind++;
			return '?';
		}
		if (pfound != NULL)
		{
			option_index = indfound;
			g_optind++;
			if (*nameend)
			{
				if (pfound->has_arg)
					g_optarg = nameend + 1;
				else
				{
					if (argv[g_optind - 1][1] == '-')
						ft_dprintf(STDERR_FILENO, "%s: option `--%s' doesn't allow an argument\n", argv[0], pfound->name);
					else
						ft_dprintf(STDERR_FILENO, "%s: option `%c%s' doesn't allow an argument\n", argv[0], argv[g_optind - 1][0], pfound->name);
					g_nextchar += strlen(g_nextchar);
					return '?';
				}
			}
			else if (pfound->has_arg == 1)
			{
				if (g_optind < argc)
					g_optarg = argv[g_optind++];
				else
				{
					ft_dprintf(STDERR_FILENO, "%s: option `%s' requires an argument\n", argv[0], argv[g_optind - 1]);
					g_nextchar += ft_strlen(g_nextchar);
					return optstring[0] == ':' ? ':' : '?';
				}
			}
			g_nextchar += ft_strlen(g_nextchar);
			if (longind != NULL)
				*longind = option_index;
			if (pfound->flag)
			{
				*(pfound->flag) = pfound->val;
				return 0;
			}
			return pfound->val;
		}
		if (!long_only || argv[g_optind][1] == '-' || ft_strchr(optstring, *g_nextchar) == NULL)
		{
			if (argv[g_optind][1] == '-')
				ft_dprintf(STDERR_FILENO, "%s: unrecognized option `--%s'\n", argv[0], g_nextchar);
			else
				ft_dprintf(STDERR_FILENO, "%s: unrecognized option `%c%s'\n", argv[0], argv[g_optind][0], g_nextchar);
			g_nextchar = (char *)"";
			g_optind++;
			return '?';
		}
	}
	char c = *g_nextchar++;
	char *temp = ft_strchr(optstring, c);

	if (*g_nextchar == '\0')
		++g_optind;
	if (temp == NULL || c == ':')
	{
		ft_dprintf(STDERR_FILENO, "%s: invalid option -- %c\n", argv[0], c);
		return '?';
	}
	if (temp[1] == ':')
	{
		if (temp[2] == ':')
		{
			if (*g_nextchar != '\0')
			{
				g_optarg = g_nextchar;
				g_optind++;
			}
			else
				g_optarg = NULL;
			g_nextchar = NULL;
		}
		else
		{
			if (*g_nextchar != '\0')
			{
				g_optarg = g_nextchar;
				g_optind++;
			}
			else if (g_optind == argc)
			{
				ft_dprintf(STDERR_FILENO, "%s: option requires an argument -- %c\n", argv[0], c);
				if (optstring[0] == ':')
					c = ':';
				else
					c = '?';
			}
			else
				g_optarg = argv[g_optind++];
			g_nextchar = NULL;
		}
	}
	return c;
}

int ft_getopt(int argc, char *const *argv, const char *optstring)
{
	return getopt_internal(argc, argv, optstring, NULL, NULL, 0);
}

int ft_getopt_long(int argc, char *const *argv, const char *options,
				   const t_option *long_options, int *opt_index)
{
	return getopt_internal(argc, argv, options, long_options, opt_index, 0);
}

// int main(int argc, char *argv[]) {
// 	int opt;

// 	for (;;) {
// 		opt = ft_getopt(argc, argv, "ab:c:de");

// 		if (opt == -1) {
// 			break;
// 		}

// 		switch (opt) {
// 			case 'a':
// 				printf("Option a\n");
// 				break;
// 			case 'e':
// 				printf("Option e\n");
// 				break;
// 			case 'd':
// 				printf("Option d\n");
// 				break;
// 			case 'b':
// 				printf("Option b with argument '%s'\n", g_optarg);
// 				break;
// 			case 'c':
// 				printf("Option c with argument '%s'\n", g_optarg);
// 				break;
// 			case '?':
// 				printf("Try 'ls --help' for more information.\n");
// 				return 1;
// 			default:
// 				// Unexpected case
// 				printf(">%d\n", opt);
// 				break;
// 		}
// 	}

// 	// Process non-option arguments (if any)
// 	for (int i = g_optind; i < argc; i++) {
// 		printf("Non-option argument: %s\n", argv[i]);
// 	}

// 	return 0;
// }

// #include <stdio.h>

// int main(int argc, char *argv[])
// {
// 	int opt;
// 	// int longindex = -1;
// 	const t_option longopts[] = {
// 		{"help", NO_ARG, NULL, 'h'},
// 		{"helpme", NO_ARG, NULL, 'H'},
// 		{"version", NO_ARG, NULL, 'v'},
// 		{"group", NO_ARG, NULL, 'g'},
// 		{"output", REQUIRED_ARG, NULL, 'o'},
// 		{"input", REQUIRED_ARG, NULL, 'i'},
// 		{NULL, 0, NULL, 0}};

// 	while ((opt = ft_getopt_long(argc, argv, "hgo:i:vl", longopts, NULL)) != -1)
// 	{
// 		switch (opt)
// 		{
// 		case 'h':
// 			printf("Usage: %s [OPTIONS]\n", argv[0]);
// 			printf("Options:\n");
// 			printf("  -h, --help\t\tDisplay this help message\n");
// 			printf("  -l, --version\t\tDisplay version information\n");
// 			printf("  -R, --output=FILE\tSpecify output file\n");
// 			printf("  -a, --input=FILE\tSpecify input file\n");
// 			printf("  -r, --input=FILE\tSpecify input file\n");
// 			printf("  -t, --input=FILE\tSpecify input file\n");
// 			return 0;
// 		case 'v':
// 			printf("Version 1.0\n");
// 			break;
// 		case 'g':
// 			printf("Group\n");
// 			break;
// 		case 'o':
// 			printf("Output file: %s\n", g_optarg);
// 			break;
// 		case 'i':
// 			printf("input file: %s\n", g_optarg);
// 			break;
// 		case 'l':
// 			printf("l\n");
// 			break;
// 		case '?':
// 			return 1;
// 		default:
// 			printf("Unknown option\n");
// 			return 1;
// 		}
// 	}

// 	return opt;
// }
