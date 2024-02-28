# ft_ls

- This project will only be evaluated by actual human beings. You are therefore free
to organize and name your files as you wish, although you need to respect some
requirements listed below.
- Your project must be written in C.
- The executable file must be named ft_ls.
- You must submit a Makefile. That Makefile will have to compile the project and
must contain the usual rules. It can only recompile the program if necessary.
- If you are clever, you will use your library for your ft_ls, submit also your folder
libft including its own Makefile at the root of your repository. Your Makefile
will have to compile the library, and then compile your project.
- You have to handle errors in a sensitive manner. In no way can your program quit
in an unexpected manner (Segmentation fault, bus error, double free, etc). If you
are unsure, handle the errors like ls.
- Your program cannot have memory leaks.

- Within your mandatory part you are allowed to use the following functions:
	- write
	- opendir : like open but for dir
	- readdir : like read but for dir
	- closedir
	- stat
	- lstat
	- getpwuid
	- getgrgid
	- listxattr
	- getxattr
	- time
	- ctime
	- readlink : read the contents of a symbolic link
	- malloc
	- free
	- perror
	- strerror
	- exit
- You are allowed to use other functions to carry out the bonus part as long as their
use is justified during your defence. For example, to use tcgetattr is justified in
certain case, to use printf because you are lazy isn’t. Be smart!

```c
// from readdir
struct dirent {
	ino_t          d_ino;       /* Inode number */
	off_t          d_off;       /* Not an offset; see below */
	unsigned short d_reclen;    /* Length of this record */
	unsigned char  d_type;      /* Type of file; not supported
									by all filesystem types */
	char           d_name[256]; /* Null-terminated filename */
};
```
`d_ino`  This is the inode number of the file.

`d_off`  The  value  returned  in d_off is the same as would be returned by calling telldir(3) at the current position in the directory stream.  Be aware that
		despite its type and name, the d_off field is seldom any kind of directory offset on modern filesystems.  Applications should treat this field as  an
		opaque value, making no assumptions about its contents; see also telldir(3).

`d_reclen`
		This is the size (in bytes) of the returned record.  This may not match the size of the structure definition shown above; see NOTES.

`d_type` This  field  contains  a value indicating the file type, making it possible to avoid the expense of calling lstat(2) if further actions depend on the
		type of the file.

		When a suitable feature test macro is defined (_DEFAULT_SOURCE since glibc 2.19, or _BSD_SOURCE on glibc 2.19 and earlier), glibc defines the following macro constants for the value returned in d_type:

		DT_BLK      This is a block device.

		DT_CHR      This is a character device.

		DT_DIR      This is a directory.

		DT_FIFO     This is a named pipe (FIFO).

		DT_LNK      This is a symbolic link.

		DT_REG      This is a regular file.>

		DT_SOCK     This is a UNIX domain socket.

		DT_UNKNOWN  The file type could not be determined.

		Currently, only some filesystems (among them: Btrfs, ext2, ext3, and ext4) have full support for returning the file type in d_type.  All applications
		must properly handle a return of DT_UNKNOWN.

`d_name` This field contains the null terminated filename.  See NOTES.


## Mandatory

- You must recode the system’s command ls.
- Its behavior must be identical to the original ls command with the following variations:
	- Amongst the numerous options available, we are asking you to create the
following: -l, -R, -a, -r and -t.
	- We strongly recommend that you account for the implications of the
option -R from the very beginning of your code...
	- You do not have to deal with the multiple column format for the exit when
the option -l isn’t in the arguments.
	- You are not required to deal with ACL and extended attributes.
	- The overall display, depending on each option, must stay as identical as possible to the system command. We will be cordial when grading either the
padding or the pagination, but no information can be missing.


### Options
-l: use a long listing format
-R: list subdirectories recursively
-a: do not ignore entries starting with .
-r: reverse order while sorting
-t: sort by time, newest first; see --time

## Bonus
Find below a few ideas of interesting bonuses you could create. Some could even be useful. You can, of course, invent your own, which will then be evaluated by your evaluators
according to their own taste.
- Management of ACL and extended attributes.
- Management of the columns without the option -l. (man 4 tty)
- Management of options -u, -f, -g, -d, ...
- Management of views in colors (Similar to option -G)
- Optimization of your code (What is the response time of your ls on a BIG ls -lR
for example?)

### Options
-u: with -lt: sort by, access time; with -l: show access time and sort by name; otherwise: sort by access time, newest first
-f: list all entries in directory order (disable sorting)
-g: like -l, but do not list owner
-d: list directories themselves, not their contents


-G: in a long listing, don't print group names