#include "ft_getopt.h"

char *g_optarg = NULL;

/* Index in ARGV of the next element to be scanned.
   This is used for communication to and from the caller
   and for communication between successive calls to `getopt'.

   On entry to `getopt', zero means this is the first call; initialize.

   When `getopt' returns -1, this is the index of the first of the
   non-option elements that the caller should itself scan.

   Otherwise, `optind' communicates from one call to the next
   how much of ARGV has been scanned so far.  */

/* 1003.2 says this must be 1 before any call.  */
int g_optind = 1;

/* Formerly, initialization of getopt depended on optind==0, which
   causes problems with re-calling getopt as programs generally don't
   know that. */

int __getopt_initialized = 0;

/* The next char to be scanned in the option-element
   in which the last option character we returned was found.
   This allows us to pick up the scan where we left off.

   If this is zero, or a null string, it means resume the scan
   by advancing to the next ARGV-element.  */

static char *nextchar;

/* Callers store zero here to inhibit the error message
   for unrecognized options.  */

/* Set to an option character which was unrecognized.
   This must be initialized on some systems to avoid linking in the
   system's own getopt implementation.  */

int g_optopt = '?';

/* Describe how to deal with options that follow non-option ARGV-elements.

   If the caller did not specify anything,
   the default is REQUIRE_ORDER if the environment variable
   POSIXLY_CORRECT is defined, PERMUTE otherwise.

   REQUIRE_ORDER means don't recognize them as options;
   stop option processing when the first non-option is seen.
   This is what Unix does.
   This mode of operation is selected by either setting the environment
   variable POSIXLY_CORRECT, or using `+' as the first character
   of the list of option characters.

   PERMUTE is the default.  We permute the contents of ARGV as we scan,
   so that eventually all the non-options are at the end.  This allows options
   to be given in any order, even with programs that were not written to
   expect this.

   RETURN_IN_ORDER is an option available to programs that were written
   to expect options and other ARGV-elements in any order and that care about
   the ordering of the two.  We describe each non-option ARGV-element
   as if it were the argument of an option with character code 1.
   Using `-' as the first character of the list of option characters
   selects this mode of operation.

   The special argument `--' forces an end of option-scanning regardless
   of the value of `ordering'.  In the case of RETURN_IN_ORDER, only
   `--' can cause `getopt' to return -1 with `optind' != ARGC.  */

static enum {
	REQUIRE_ORDER,
	PERMUTE,
	RETURN_IN_ORDER
} ordering;

static int first_nonopt;
static int last_nonopt;

/* Exchange two adjacent subsequences of ARGV.
   One subsequence is elements [first_nonopt,last_nonopt)
   which contains all the non-options that have been skipped so far.
   The other is elements [last_nonopt,optind), which contains all
   the options processed since those non-options were skipped.

   `first_nonopt' and `last_nonopt' are relocated so that they describe
   the new indices of the non-options in ARGV after they are moved.  */

static void
exchange(char **argv)
{
	int bottom = first_nonopt;
	int middle = last_nonopt;
	int top = g_optind;
	char *tem;

	/* Exchange the shorter segment with the far end of the longer segment.
	   That puts the shorter segment into the right place.
	   It leaves the longer segment in the right place overall,
	   but it consists of two parts that need to be swapped next.  */

	while (top > middle && middle > bottom)
	{
		if (top - middle > middle - bottom)
		{
			/* Bottom segment is the short one.  */
			int len = middle - bottom;
			register int i;

			/* Swap it with the top part of the top segment.  */
			for (i = 0; i < len; i++)
			{
				tem = argv[bottom + i];
				argv[bottom + i] = argv[top - (middle - bottom) + i];
				argv[top - (middle - bottom) + i] = tem;
			}
			/* Exclude the moved bottom segment from further swapping.  */
			top -= len;
		}
		else
		{
			/* Top segment is the short one.  */
			int len = top - middle;
			register int i;

			/* Swap it with the bottom part of the bottom segment.  */
			for (i = 0; i < len; i++)
			{
				tem = argv[bottom + i];
				argv[bottom + i] = argv[middle + i];
				argv[middle + i] = tem;
			}
			/* Exclude the moved top segment from further swapping.  */
			bottom += len;
		}
	}

	/* Update records for the slots the non-options now occupy.  */

	first_nonopt += (g_optind - last_nonopt);
	last_nonopt = g_optind;
}

static const char *
_getopt_initialize(
	const char *optstring)
{
	/* Start processing options with ARGV-element 1 (since ARGV-element 0
	   is the program name); the sequence of previously skipped
	   non-option ARGV-elements is empty.  */

	first_nonopt = last_nonopt = g_optind;

	nextchar = NULL;

	/* Determine how to handle the ordering of options and nonoptions.  */

	if (optstring[0] == '-')
	{
		ordering = RETURN_IN_ORDER;
		++optstring;
	}
	else if (optstring[0] == '+')
	{
		ordering = REQUIRE_ORDER;
		++optstring;
	}
	else
		ordering = PERMUTE;

	return optstring;
}

/* Scan elements of ARGV (whose length is ARGC) for option characters
   given in OPTSTRING.

   If an element of ARGV starts with '-', and is not exactly "-" or "--",
   then it is an option element.  The characters of this element
   (aside from the initial '-') are option characters.  If `getopt'
   is called repeatedly, it returns successively each of the option characters
   from each of the option elements.

   If `getopt' finds another option character, it returns that character,
   updating `optind' and `nextchar' so that the next call to `getopt' can
   resume the scan with the following option character or ARGV-element.

   If there are no more option characters, `getopt' returns -1.
   Then `optind' is the index in ARGV of the first ARGV-element
   that is not an option.  (The ARGV-elements have been permuted
   so that those that are not options now come last.)

   OPTSTRING is a string containing the legitimate option characters.
   If an option character is seen that is not listed in OPTSTRING,
   return '?' after printing an error message.  If you set `opterr' to
   zero, the error message is suppressed but we still return '?'.

   If a char in OPTSTRING is followed by a colon, that means it wants an arg,
   so the following text in the same ARGV-element, or the text of the following
   ARGV-element, is returned in `optarg'.  Two colons mean an option that
   wants an optional arg; if there is text in the current ARGV-element,
   it is returned in `optarg', otherwise `optarg' is set to zero.

   If OPTSTRING starts with `-' or `+', it requests different methods of
   handling the non-option ARGV-elements.
   See the comments about RETURN_IN_ORDER and REQUIRE_ORDER, above.

   Long-named options begin with `--' instead of `-'.
   Their names may be abbreviated as long as the abbreviation is unique
   or is an exact match for some defined option.  If they have an
   argument, it follows the option name in the same ARGV-element, separated
   from the option name by a `=', or else the in next ARGV-element.
   When `getopt' finds a long-named option, it returns 0 if that option's
   `flag' field is nonzero, the value of the option's `val' field
   if the `flag' field is zero.

   The elements of ARGV aren't really const, because we permute them.
   But we pretend they're const in the prototype to be compatible
   with other systems.

   LONGOPTS is a vector of `t_option' terminated by an
   element containing a name which is zero.

   LONGIND returns the index in LONGOPT of the long-named option found.
   It is only valid when a long-named option has been found by the most
   recent call.

   If LONG_ONLY is nonzero, '-' as well as '--' can introduce
   long-named options.  */

int _getopt_internal(int argc, char *const *argv, const char *optstring,
					 const t_option *longopts,
					 int *longind, int long_only)
{
	char *nameend;
	const t_option *p;
	const t_option *pfound = NULL;
	int exact = 0;
	int ambig = 0;
	int indfound = 0;
	int option_index;

	g_optarg = NULL;

	if (g_optind == 0 || !__getopt_initialized)
	{
		if (g_optind == 0)
			g_optind = 1; /* Don't scan ARGV[0], the program name.  */
		optstring = _getopt_initialize(optstring);
		__getopt_initialized = 1;
	}

	/* Test whether ARGV[optind] points to a non-option argument.
	   Either it does not have option syntax, or there is an environment flag
	   from the shell indicating it is not an option.  The later information
	   is only used when the used in the GNU libc.  */

	if (nextchar == NULL || *nextchar == '\0')
	{
		/* Advance to the next ARGV-element.  */

		/* Give FIRST_NONOPT & LAST_NONOPT rational values if OPTIND has been
	   moved back by the user (who may also have changed the arguments).  */
		if (last_nonopt > g_optind)
			last_nonopt = g_optind;
		if (first_nonopt > g_optind)
			first_nonopt = g_optind;

		if (ordering == PERMUTE)
		{
			/* If we have just processed some options following some non-options,
			   exchange them so that the options come first.  */

			if (first_nonopt != last_nonopt && last_nonopt != g_optind)
				exchange((char **)argv);
			else if (last_nonopt != g_optind)
				first_nonopt = g_optind;

			/* Skip any additional non-options
			   and extend the range of non-options previously skipped.  */

			while (g_optind < argc && (argv[g_optind][0] != '-' || argv[g_optind][1] == '\0'))
				g_optind++;
			last_nonopt = g_optind;
		}

		/* The special ARGV-element `--' means premature end of options.
	   Skip it like a null option,
	   then exchange with previous non-options as if it were an option,
	   then skip everything else like a non-option.  */

		if (g_optind != argc && !ft_strcmp(argv[g_optind], "--"))
		{
			g_optind++;

			if (first_nonopt != last_nonopt && last_nonopt != g_optind)
				exchange((char **)argv);
			else if (first_nonopt == last_nonopt)
				first_nonopt = g_optind;
			last_nonopt = argc;

			g_optind = argc;
		}

		/* If we have done all the ARGV-elements, stop the scan
	   and back over any non-options that we skipped and permuted.  */

		if (g_optind == argc)
		{
			/* Set the next-arg-index to point at the non-options
			   that we previously skipped, so the caller will digest them.  */
			if (first_nonopt != last_nonopt)
				g_optind = first_nonopt;
			return -1;
		}

		/* If we have come to a non-option and did not permute it,
	   either stop the scan or describe it to the caller and pass it by.  */

		if (argv[g_optind][0] != '-' || argv[g_optind][1] == '\0')
		{
			if (ordering == REQUIRE_ORDER)
				return -1;
			g_optarg = argv[g_optind++];
			return 1;
		}

		/* We have found another option-ARGV-element.
	   Skip the initial punctuation.  */

		nextchar = (argv[g_optind] + 1 + (longopts != NULL && argv[g_optind][1] == '-'));
	}

	/* Decode the current option-ARGV-element.  */

	/* Check whether the ARGV-element is a long option.

	   If long_only and the ARGV-element has the form "-f", where f is
	   a valid short option, don't consider it an abbreviated form of
	   a long option that starts with f.  Otherwise there would be no
	   way to give the -f short option.

	   On the other hand, if there's a long option "fubar" and
	   the ARGV-element is "-fu", do consider that an abbreviation of
	   the long option, just like "--fu", and not "-f" with arg "u".

	   This distinction seems to be the most useful approach.  */

	if (longopts != NULL && (argv[g_optind][1] == '-' || (long_only && (argv[g_optind][2] || !ft_strchr(optstring, argv[g_optind][1])))))
	{

		for (nameend = nextchar; *nameend && *nameend != '='; nameend++)
			/* Do nothing.  */;

		/* Test all long options for either exact match
	   or abbreviated matches.  */
		for (p = longopts, option_index = 0; p->name; p++, option_index++)
			if (!ft_strncmp(p->name, nextchar, nameend - nextchar))
			{
				if ((unsigned int)(nameend - nextchar) == (unsigned int)ft_strlen(p->name))
				{
					/* Exact match found.  */
					pfound = p;
					indfound = option_index;
					exact = 1;
					break;
				}
				else if (pfound == NULL)
				{
					/* First nonexact match found.  */
					pfound = p;
					indfound = option_index;
				}
				else
					/* Second or later nonexact match found.  */
					ambig = 1;
			}

		if (ambig && !exact)
		{
			ft_dprintf(STDERR_FILENO, "%s: option `%s' is ambiguous\n",
					   argv[0], argv[g_optind]);
			nextchar += ft_strlen(nextchar);
			g_optind++;
			g_optopt = 0;
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
						/* --option */
						ft_dprintf(STDERR_FILENO,
								   "%s: option `--%s' doesn't allow an argument\n",
								   argv[0], pfound->name);
					else
						/* +option or -option */
						ft_dprintf(STDERR_FILENO,
								   "%s: option `%c%s' doesn't allow an argument\n",
								   argv[0], argv[g_optind - 1][0], pfound->name);

					nextchar += strlen(nextchar);

					g_optopt = pfound->val;
					return '?';
				}
			}
			else if (pfound->has_arg == 1)
			{
				if (g_optind < argc)
					g_optarg = argv[g_optind++];
				else
				{
					ft_dprintf(STDERR_FILENO,
							   "%s: option `%s' requires an argument\n",
							   argv[0], argv[g_optind - 1]);
					nextchar += ft_strlen(nextchar);
					g_optopt = pfound->val;
					return optstring[0] == ':' ? ':' : '?';
				}
			}
			nextchar += ft_strlen(nextchar);
			if (longind != NULL)
				*longind = option_index;
			if (pfound->flag)
			{
				*(pfound->flag) = pfound->val;
				return 0;
			}
			return pfound->val;
		}

		/* Can't find it as a long option.  If this is not getopt_long_only,
	   or the option starts with '--' or is not a valid short
	   option, then it's an error.
	   Otherwise interpret it as a short option.  */
		if (!long_only || argv[g_optind][1] == '-' || ft_strchr(optstring, *nextchar) == NULL)
		{
			if (argv[g_optind][1] == '-')
				/* --option */
				ft_dprintf(STDERR_FILENO, "%s: unrecognized option `--%s'\n",
						   argv[0], nextchar);
			else
				/* +option or -option */
				ft_dprintf(STDERR_FILENO, "%s: unrecognized option `%c%s'\n",
						   argv[0], argv[g_optind][0], nextchar);
			nextchar = (char *)"";
			g_optind++;
			g_optopt = 0;
			return '?';
		}
	}

	/* Look at and handle the next short option-character.  */

	{
		char c = *nextchar++;
		char *temp = ft_strchr(optstring, c);

		/* Increment `optind' when we start to process its last character.  */
		if (*nextchar == '\0')
			++g_optind;

		if (temp == NULL || c == ':')
		{
			ft_dprintf(STDERR_FILENO, "%s: invalid option -- %c\n",
					   argv[0], c);
			g_optopt = c;
			return '?';
		}
		/* Convenience. Treat POSIX -W foo same as long option --foo */
		if (temp[0] == 'W' && temp[1] == ';')
		{

			/* This is an option that requires an argument.  */
			if (*nextchar != '\0')
			{
				g_optarg = nextchar;
				/* If we end this ARGV-element by taking the rest as an arg,
				we must advance to the next element now.  */
				g_optind++;
			}
			else if (g_optind == argc)
			{
				ft_dprintf(STDERR_FILENO, "%s: option requires an argument -- %c\n", argv[0], c);
				g_optopt = c;
				if (optstring[0] == ':')
					c = ':';
				else
					c = '?';
				return c;
			}
			else
				/* We already incremented `optind' once;
				increment it again when taking next ARGV-elt as argument.  */
				g_optarg = argv[g_optind++];

			/* optarg is now the argument, see if it's in the table of longopts.  */

			for (nextchar = nameend = g_optarg; *nameend && *nameend != '='; nameend++)
				/* Do nothing.  */;

			/* Test all long options for either exact match or abbreviated matches.  */
			for (p = longopts, option_index = 0; p->name; p++, option_index++)
				if (!ft_strncmp(p->name, nextchar, nameend - nextchar))
				{
					if ((unsigned int)(nameend - nextchar) == ft_strlen(p->name))
					{
						/* Exact match found.  */
						pfound = p;
						indfound = option_index;
						exact = 1;
						break;
					}
					else if (pfound == NULL)
					{
						/* First nonexact match found.  */
						pfound = p;
						indfound = option_index;
					}
					else
						/* Second or later nonexact match found.  */
						ambig = 1;
				}
			if (ambig && !exact)
			{
				ft_dprintf(STDERR_FILENO, "%s: option `-W %s' is ambiguous\n", argv[0], argv[g_optind]);
				nextchar += ft_strlen(nextchar);
				g_optind++;
				return '?';
			}
			if (pfound != NULL)
			{
				option_index = indfound;
				if (*nameend)
				{
					if (pfound->has_arg)
						g_optarg = nameend + 1;
					else
					{
						ft_dprintf(STDERR_FILENO, "%s: option `-W %s' doesn't allow an argument\n", argv[0], pfound->name);
						nextchar += ft_strlen(nextchar);
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
						nextchar += ft_strlen(nextchar);
						return optstring[0] == ':' ? ':' : '?';
					}
				}
				nextchar += ft_strlen(nextchar);
				if (longind != NULL)
					*longind = option_index;
				if (pfound->flag)
				{
					*(pfound->flag) = pfound->val;
					return 0;
				}
				return pfound->val;
			}
			nextchar = NULL;
			return 'W'; /* Let the application handle it.   */
		}
		if (temp[1] == ':')
		{
			if (temp[2] == ':')
			{
				/* This is an option that accepts an argument optionally.  */
				if (*nextchar != '\0')
				{
					g_optarg = nextchar;
					g_optind++;
				}
				else
					g_optarg = NULL;
				nextchar = NULL;
			}
			else
			{
				/* This is an option that requires an argument.  */
				if (*nextchar != '\0')
				{
					g_optarg = nextchar;
					/* If we end this ARGV-element by taking the rest as an arg,
					   we must advance to the next element now.  */
					g_optind++;
				}
				else if (g_optind == argc)
				{
					ft_dprintf(STDERR_FILENO, "%s: option requires an argument -- %c\n", argv[0], c);
					g_optopt = c;
					if (optstring[0] == ':')
						c = ':';
					else
						c = '?';
				}
				else
					/* We already incremented `optind' once;
				   increment it again when taking next ARGV-elt as argument.  */
					g_optarg = argv[g_optind++];
				nextchar = NULL;
			}
		}
		return c;
	}
}

int ft_getopt(int argc, char *const *argv, const char *optstring)
{
	return _getopt_internal(argc, argv, optstring, NULL, NULL, 0);
}

int ft_getopt_long(int argc, char *const *argv, const char *options,
				   const t_option *long_options, int *opt_index)
{
	return _getopt_internal(argc, argv, options, long_options, opt_index, 0);
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

// int main(int argc, char *argv[])
// {
// 	int opt;
// 	// int longindex = -1;
// 	const t_option longopts[] = {
// 		{"help", NO_ARG, NULL, 'h'},
// 		{"help", NO_ARG, NULL, 'H'},
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
