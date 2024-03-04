

#include <error.h>
#include "libft.h"
#include "ft_argmatch.h"


/* When reporting an invalid argument, show nonprinting characters
   by using the quoting style ARGMATCH_QUOTING_STYLE.  Do not use
   literal_quoting_style.  */
#ifndef ARGMATCH_QUOTING_STYLE
#define ARGMATCH_QUOTING_STYLE locale_quoting_style
#endif

/* Non failing version of argmatch call this function after failing. */

static void
__argmatch_die(void)
{
	exit(EXIT_FAILURE);
}


argmatch_exit_fn argmatch_die = __argmatch_die;

ptrdiff_t argmatch(const char *arg, const char *const *arglist, \
	const void *vallist, size_t valsize)
{
	size_t i;				 /* Temporary index in ARGLIST.  */
	size_t arglen;			 /* Length of ARG.  */
	ptrdiff_t matchind = -1; /* Index of first nonexact match.  */
	bool ambiguous = false;	 /* If true, multiple nonexact match(es).  */

	arglen = ft_strlen(arg);

	/* Test all elements for either exact match or abbreviated matches.  */
	for (i = 0; arglist[i]; i++)
	{
		if (!ft_strncmp(arglist[i], arg, arglen))
		{
			if (ft_strlen(arglist[i]) == arglen)
				/* Exact match found.  */
				return i;
			else if (matchind == -1)
				/* First nonexact match found.  */
				matchind = i;
			else
			{
				/* Second nonexact match found.  */
				if (vallist == NULL || ft_memcmp((char const *)vallist + valsize * matchind,
											  (char const *)vallist + valsize * i, valsize))
				{
					/* There is a real ambiguity, or we could not
					   disambiguate. */
					ambiguous = true;
				}
			}
		}
	}
	if (ambiguous)
		return -2;
	else
		return matchind;
}

ptrdiff_t argmatch_exact(const char *arg, const char *const *arglist)
{
	size_t i;

	/* Test elements for exact match.  */
	for (i = 0; arglist[i]; i++)
	{
		if (!ft_strcmp(arglist[i], arg))
			return i;
	}

	return -1;
}

void argmatch_invalid(const char *context, const char *value, ptrdiff_t problem)
{
	char const *format = (problem == -1
							  ? "invalid argument %s for %s"
							  : "ambiguous argument %s for %s");

	// error(0, 0, format, quotearg_n_style(0, 2, value),
	// 	  quote_n(1, context));
	ft_dprintf(STDERR_FILENO, format, ft_quote(value), ft_quote(context));
	exit(EXIT_FAILURE);
}


void argmatch_valid(const char *const *arglist,
					const void *vallist, size_t valsize)
{
	size_t i;
	const char *last_val = NULL;

	/* We try to put synonyms on the same line.  The assumption is that
	   synonyms follow each other */
	ft_putstr_fd("Valid arguments are:", STDERR_FILENO);
	for (i = 0; arglist[i]; i++)
		if ((i == 0) || ft_memcmp(last_val, (char const *)vallist + valsize * i, valsize))
		{
			char * args = ft_quote(arglist[i]);
			ft_dprintf(STDERR_FILENO, "\n  - %s", args);
			free(args);
			last_val = (char const *)vallist + valsize * i;
		}
		else
		{
			char * args = ft_quote(arglist[i]);
			ft_dprintf(STDERR_FILENO, ", %s", args);
			free(args);
		}
	ft_putchar_fd('\n', STDERR_FILENO);
}

/* Never failing versions of the previous functions.

   CONTEXT is the context for which argmatch is called (e.g.,
   "--version-control", or "$VERSION_CONTROL" etc.).  Upon failure,
   calls the (supposed never to return) function EXIT_FN. */

ptrdiff_t
__xargmatch_internal(const char *context,
					 const char *arg, const char *const *arglist,
					 const void *vallist, size_t valsize,
					 argmatch_exit_fn exit_fn,
					 bool allow_abbreviation)
{
	ptrdiff_t res;

	if (allow_abbreviation)
		res = argmatch(arg, arglist, vallist, valsize);
	else
		res = argmatch_exact(arg, arglist);

	if (res >= 0)
		/* Success. */
		return res;

	/* We failed.  Explain why. */
	argmatch_invalid(context, arg, res);
	argmatch_valid(arglist, vallist, valsize);
	(*exit_fn)();

	return -1; /* To please the compilers. */
}

/* Look for VALUE in VALLIST, an array of objects of size VALSIZE and
   return the first corresponding argument in ARGLIST */
const char *
argmatch_to_argument(const void *value,
					 const char *const *arglist,
					 const void *vallist, size_t valsize)
{
	size_t i;

	for (i = 0; arglist[i]; i++)
		if (!ft_memcmp(value, (char const *)vallist + valsize * i, valsize))
			return arglist[i];
	return NULL;
}