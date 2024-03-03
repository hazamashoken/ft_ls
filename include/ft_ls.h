/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <tliangso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 21:13:02 by tliangso          #+#    #+#             */
/*   Updated: 2024/03/04 00:43:57 by tliangso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H

# include <fcntl.h>
# include <sys/stat.h>
# include <time.h>
# include <sys/types.h>
# include <dirent.h>
# include <grp.h>
# include <pwd.h>
# include <stdbool.h>
# include <linux/limits.h>

# define SECONDS_PER_DAY (24 * 60 * 60)
# define MONTHS_PER_YEAR 12

# define HYPERLINK(path) ft_printf("\e]8;;%s\e\\%s\e]8;;\e\\\n", path, getbasename(path));

# define HELP "\
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

enum e_long_options {
	O_HELP =  200
};

typedef struct s_entry
{
	// file name
	char	name[NAME_MAX];
	// symbolic link name
	char	*linkname;
	// absolute path name for hyperlink
	char	*absolute_name;

	struct stat stat;

	enum e_filetype filetype;

	mode_t	linkmode;

	bool stat_ok;

	bool link_ok;

	int	quoted;



	off_t	size;
	mode_t	mode;
	time_t	atime;
	time_t	mtime;
	time_t	ctime;
	nlink_t	nlink;
	uid_t	uid;
	gid_t	gid;
	dev_t	dev;
	ino_t	ino;
	long	blksize;
	blkcnt_t	blocks;

	struct s_entry *childern;
}			t_entry;

typedef struct s_ls_option
{
	char ignore_mode;
	
	char sort_opt;
	char format_opt;

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
	
}	t_ls_option;

void		display_date(struct stat file_stat);

#endif