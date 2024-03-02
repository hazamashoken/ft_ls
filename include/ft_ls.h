/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <tliangso@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 21:13:02 by tliangso          #+#    #+#             */
/*   Updated: 2024/03/02 01:51:37 by tliangso         ###   ########.fr       */
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

/*
The struct stat is a data structure commonly used in Unix-like systems, including Linux, to store detailed information about a file. It's typically used by system calls like stat(), fstat(), and lstat() to retrieve various file attributes. Here's a breakdown of what struct stat contains:

File Properties:

	Device IDs:
		st_dev: Device ID of the device containing the file.
		st_rdev: Device ID (relevant for character and block special devices).
	File Identification:
		st_ino: Unique file serial number (inode).
		st_nlink: Number of hard links to the file.
	Ownership:
		st_uid: User ID of the file owner.
		st_gid: Group ID of the group owning the file.
	Permissions:
		st_mode: File type and access permissions (e.g., regular file, directory, permissions for owner, group, and others).
	Sizes:
		st_size: Size of the file in bytes (relevant for regular files).
		st_blksize: Optimal block size for I/O operations on the file system.
	Timestamps:
		st_atime: Last access time (may not reflect the most recent access).
		st_mtime: Last modification time.
		st_ctime: Last change time (e.g., metadata change).

*/
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

void		display_date(struct stat file_stat);

#endif