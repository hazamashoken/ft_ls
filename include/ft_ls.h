/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <tliangso@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 21:13:02 by tliangso          #+#    #+#             */
/*   Updated: 2024/03/05 00:50:47 by tliangso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H

#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <stdbool.h>
#include <linux/limits.h>

#include "ft_quote.h"

#define SECONDS_PER_DAY (24 * 60 * 60)
#define MONTHS_PER_YEAR 12

#define HYPERLINK(path) ft_printf("\e]8;;%s\e\\%s\e]8;;\e\\\n", path, getbasename(path));

#define HELP "\
Usage: ft_ls [OPTION]... [FILE]...\n\
List information about the FILEs (the current directory by default).\n\
Sort entries alphabetically if none of -cftuvSUX nor --sort is specified.\n\
\n\
Mandatory arguments to long options are mandatory for short options too.\n\
	-a, --all             do not ignore entries starting with .\n\
	-A, --almost-all      do not list implied . and ..\n\
	-d, --directory       list directories themselves, not their contents\n\
	-f                    list all entries in directory order\n\
	-g                    like -l, but do not list owner\n\
	-G, --no-group        in a long listing, don't print group names\n\
	-l                    use a long listing format\n\
	-r, --reverse         reverse order while sorting\n\
	-R, --recursive       list subdirectories recursively\n\
	-t                    sort by modification time, newest first\n\
	-u                    with -lt: sort by, and show, access time, newest first\n\
	-1                    list one file per line\n\
	  --help        display this help and exit\n\
\n\
Exit status:\n\
 0  if OK,\n\
 1  if minor problems (e.g., cannot access subdirectory),\n\
 2  if serious trouble (e.g., cannot access command-line argument)\
"

typedef struct stat t_stat;

enum e_filetype
{
	unknown,
	fifo,
	chardev,
	directory,
	blockdev,
	normal,
	symbolic_link,
	sock,
	whiteout,
	arg_directory
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
enum
{
	AUTHOR_OPTION = CHAR_MAX + 1,
	BLOCK_SIZE_OPTION,
	COLOR_OPTION,
	DEREFERENCE_COMMAND_LINE_SYMLINK_TO_DIR_OPTION,
	FILE_TYPE_INDICATOR_OPTION,
	FORMAT_OPTION,
	FULL_TIME_OPTION,
	GROUP_DIRECTORIES_FIRST_OPTION,
	HIDE_OPTION,
	HYPERLINK_OPTION,
	INDICATOR_STYLE_OPTION,
	QUOTING_STYLE_OPTION,
	SHOW_CONTROL_CHARS_OPTION,
	SI_OPTION,
	SORT_OPTION,
	TIME_OPTION,
	TIME_STYLE_OPTION,
	ZERO_OPTION,
	HELP_OPTION,
};

typedef enum e_format
{
	long_format,   /* -l and other options that imply -l */
	one_per_line,  /* -1 */
	many_per_line, /* -C */
	horizontal,	   /* -x */
	with_commas	   /* -m */
} t_ls_format;

enum time_type
{
	time_mtime = 0, /* default */
	time_ctime,		/* -c */
	time_atime,		/* -u */
	time_btime,		/* birth time */
	time_numtypes	/* the number of elements of this enum */
};

typedef enum e_sort_type
{
	sort_name = 0,	/* default */
	sort_extension, /* -X */
	sort_width,
	sort_size,	  /* -S */
	sort_version, /* -v */
	sort_time,	  /* -t; must be second to last */
	sort_none,	  /* -U; must be last */
	sort_numtypes /* the number of elements of this enum */
} t_sort_type;

enum when_type
{
	when_never,	 /* 0: default or --color=never */
	when_always, /* 1: --color=always */
	when_if_tty, /* 2: --color=tty */
};

char const *const when_args[] =
{
	/* force and none are for compatibility with another color-ls version */
	"always", "yes", "force",
	"never", "no", "none",
	"auto", "tty", "if-tty",
	NULL
};

enum when_type const when_types[] =
{
	when_always, when_always, when_always,
	when_never, when_never, when_never,
	when_if_tty, when_if_tty, when_if_tty,
};

enum indicator_style
{
	none = 0,  /*     --indicator-style=none (default) */
	slash,	   /* -p, --indicator-style=slash */
	file_type, /*     --indicator-style=file-type */
	classify   /* -F, --indicator-style=classify */
};

char const *const indicator_style_args[] =
{
	"none", "slash", "file-type", "classify", NULL
};

enum indicator_style const indicator_style_types[] =
{
	none, slash, file_type, classify
};

char const *const format_args[] =
{
  "verbose", "long", "commas", "horizontal", "across",
  "vertical", "single-column", NULL
};

t_ls_format const format_types[] =
{
  long_format, long_format, with_commas, horizontal, horizontal,
  many_per_line, one_per_line
};

char const *const sort_args[] =
{
  "none", "time", "size", "extension", "version", "width", NULL
};

t_sort_type const sort_types[] =
{
  sort_none, sort_time, sort_size, sort_extension, sort_version, sort_width
};

// These should not be here
/* Correspondences to quoting style names.  */
enum quoting_style const quoting_style_vals[] =
{
  literal_quoting_style,
  shell_quoting_style,
  shell_always_quoting_style,
  shell_escape_quoting_style,
  shell_escape_always_quoting_style,
  c_quoting_style,
  c_maybe_quoting_style,
  escape_quoting_style,
  locale_quoting_style,
  clocale_quoting_style
};

/* The quoting options used by quote_n and quote.  Its type is incomplete,
   so it's useful only in expressions like '&quote_quoting_options'.  */
struct quoting_options quote_quoting_options =
  {
    locale_quoting_style,
    0,
    { 0 },
    NULL, NULL
  };

/* Names of quoting styles.  */
char const *const quoting_style_args[] =
{
  "literal",
  "shell",
  "shell-always",
  "shell-escape",
  "shell-escape-always",
  "c",
  "c-maybe",
  "escape",
  "locale",
  "clocale",
  NULL
};

struct ignore_pattern
  {
    char const *pattern;
    struct ignore_pattern *next;
  };

struct ignore_pattern *hide_patterns;

typedef enum
{
	// Ignore hidden file and files specified by --hide and --ignore
	IGNORE_DEFAULT = 0,

	// Ignore '.' and '..' and files specified by --ignore
	IGNORE_DOT_AND_DOTDOT,

	// Ignore only files specified by --ignore
	IGNORE_MINIMAL,
} t_ignore_mode;

typedef struct s_entry
{
	// file name
	char name[NAME_MAX];
	// symbolic link name
	char *linkname;
	// absolute path name for hyperlink
	char *absolute_name;

	struct stat stat;

	enum e_filetype filetype;

	mode_t linkmode;

	bool stat_ok;

	bool link_ok;

	int quoted;

	off_t size;
	mode_t mode;
	time_t atime;
	time_t mtime;
	time_t ctime;
	nlink_t nlink;
	uid_t uid;
	gid_t gid;
	dev_t dev;
	ino_t ino;
	long blksize;
	blkcnt_t blocks;

	struct s_entry *childern;
} t_entry;

typedef struct s_ls_option
{
	t_ignore_mode ignore_mode;

	char sort_opt;
	t_ls_format format_opt;

	char quoting_style_opt;
	char indicator_style;
	char time_type;
	char numeric_ids;
	char directories_first;

	char immediate_dirs;

	char print_with_color;
	char print_hyperlink;
	char print_block_size;
	char print_owner;
	char print_inode;
	char print_group;
	char print_author;

	char width_opt;

	char eolbyte;

	char recursive;
	char sort_reverse;


} t_ls_option;

void display_date(struct stat file_stat);

#endif