#ifndef FT_LS_H

/*
OPT_LONG_LIST	: -l
OPT_RECURSIVE	: -R / --recursive
OPT_ALL			: -a / --all
OPT_REVERSE		: -r / --reverse
OPT_TIME		: -t

// BONUS
OPT_UACCESS		: -u
OPT_NOSORT		: -f
OPT_NOOWNER		: -g
OPT_NOGROUP		: -G / --no-group
OPT_DIRONLY		: -d / --directory

// EXTRA
OPT_ALMOSTALL	: -A / --almost-all
OPT_AUTHOR		: --author



*/
enum e_options
{
	OPT_LONG_LIST,
	OPT_RECURSIVE,
	OPT_ALL,
	OPT_REVERSE,
	OPT_TIME,

	OPT_UACCESS,
	OPT_NOSORT,
	OPT_NOOWNER,
	OPT_NOGROUP,
	OPT_DIRONLY,
};

#endif