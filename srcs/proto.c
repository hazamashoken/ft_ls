// # include "libft.h"
// # include "ft_ls.h"
// # include "ft_getopt.h"

// /* Display letters and indicators for each filetype.
// 	Keep these in sync with enum filetype.  */
// static char const filetype_letter[] = "?pcdb-lswd";

// /* Initial size of hash table.
// 	Most hierarchies are likely to be shallower than this.  */
// #define INITIAL_TABLE_SIZE 30


// /* The set of 'active' directories, from the current command-line argument
// 	to the level in the hierarchy at which files are being listed.
// 	A directory is represented by its device and inode numbers (struct dev_ino).
// 	A directory is added to this set when ls begins listing it or its
// 	entries, and it is removed from the set just after ls has finished
// 	processing it.  This set is used solely to detect loops, e.g., with
// 	mkdir loop; cd loop; ln -s ../loop sub; ls -RL  */
// static Hash_table *active_dir_set;

// #define LOOP_DETECT (!!active_dir_set)

// /* The table of files in the current directory:

// 	'cwd_file' points to a vector of 'struct fileinfo', one per file.
// 	'cwd_n_alloc' is the number of elements space has been allocated for.
// 	'cwd_n_used' is the number actually in use.  */

// /* Address of block containing the files that are described.  */
// static struct fileinfo *cwd_file;

// /* Length of block that 'cwd_file' points to, measured in files.  */
// static size_t cwd_n_alloc;

// /* Index of first unused slot in 'cwd_file'.  */
// static size_t cwd_n_used;

// /* Whether files needs may need padding due to quoting.  */
// static bool cwd_some_quoted;

// /* Whether quoting style _may_ add outer quotes,
// 	and whether aligning those is useful.  */
// static bool align_variable_outer_quotes;

// /* Vector of pointers to files, in proper sorted order, and the number
// 	of entries allocated for it.  */
// static void **sorted_file;
// static size_t sorted_file_alloc;

// /* When true, in a color listing, color each symlink name according to the
// 	type of file it points to.  Otherwise, color them according to the 'ln'
// 	directive in LS_COLORS.  Dangling (orphan) symlinks are treated specially,
// 	regardless.  This is set when 'ln=target' appears in LS_COLORS.  */

// static bool color_symlink_as_referent;

// static char const *hostname;

// /* mode of appropriate file for colorization */
// #define FILE_OR_LINK_MODE(File) \
// 	((color_symlink_as_referent && (File)->linkok) \
// 	 ? (File)->linkmode : (File)->stat.st_mode)

// /* Record of one pending directory waiting to be listed.  */
// static t_pending *pending_dirs;

// /* Current time in seconds and nanoseconds since 1970, updated as
// 	needed when deciding whether a file is recent.  */
// static struct timespec current_time;

// static bool print_scontext;
// static char UNKNOWN_SECURITY_CONTEXT[] = "?";

// /* Whether any of the files has an ACL.  This affects the width of the
// 	mode column.  */
// static bool any_has_acl;

// /* The number of columns to use for columns containing inode numbers,
// 	block sizes, link counts, owners, groups, authors, major device
// 	numbers, minor device numbers, and file sizes, respectively.  */
// static int inode_number_width;
// static int block_size_width;
// static int nlink_width;
// static int scontext_width;
// static int owner_width;
// static int group_width;
// static int author_width;
// static int major_device_number_width;
// static int minor_device_number_width;
// static int file_size_width;

// /* Option flags */

// /* long_format for lots of info, one per line.
// 	one_per_line for just names, one per line.
// 	many_per_line for just names, many per line, sorted vertically.
// 	horizontal for just names, many per line, sorted horizontally.
// 	with_commas for just names, many per line, separated by commas.

// 	-l (and other options that imply -l), -1, -C, -x and -m control
// 	this parameter.  */
// static enum e_time_style format;

// static char const *const time_style_args[] =
// {
//   "full-iso", "long-iso", "iso", "locale", NULL
// };

// /* 'full-iso' uses full ISO-style dates and times.  'long-iso' uses longer
// 	ISO-style timestamps, though shorter than 'full-iso'.  'iso' uses shorter
// 	ISO-style timestamps.  'locale' uses locale-dependent timestamps.  */
// static enum e_time_style const time_style_types[] =
// {
//   full_iso_time_style, long_iso_time_style, iso_time_style,
//   locale_time_style
// };

// /* Type of time to print or sort by.  Controlled by -c and -u.
// 	The values of each item of this enum are important since they are
// 	used as indices in the sort functions array (see sort_files()).  */
// static enum e_time_type time_type;

// /* The file characteristic to sort by.  Controlled by -t, -S, -U, -X, -v.
// 	The values of each item of this enum are important since they are
// 	used as indices in the sort functions array (see sort_files()).  */
// static enum e_sort_type sort_type;

// /* Direction of sort.
// 	false means highest first if numeric,
// 	lowest first if alphabetic;
// 	these are the defaults.
// 	true means the opposite order in each case.  -r  */
// static bool sort_reverse;

// /* True means to display owner information.  -g turns this off.  */
// static bool print_owner = true;

// /* True means to display author information.  */
// static bool print_author;

// /* True means to display group information.  -G and -o turn this off.  */
// static bool print_group = true;

// /* True means print the user and group id's as numbers rather
// 	than as names.  -n  */
// static bool numeric_ids;

// /* True means mention the size in blocks of each file.  -s  */
// static bool print_block_size;

// /* Human-readable options for output, when printing block counts.  */
// static int human_output_opts;

// /* The units to use when printing block counts.  */
// static uintmax_t output_block_size;

// /* Likewise, but for file sizes.  */
// static int file_human_output_opts;
// static uintmax_t file_output_block_size = 1;

// /* Follow the output with a special string.  Using this format,
// 	Emacs' dired mode starts up twice as fast, and can handle all
// 	strange characters in file names.  */
// static bool dired;

// /* 'none' means don't mention the type of files.
// 	'slash' means mention directories only, with a '/'.
// 	'file_type' means mention file types.
// 	'classify' means mention file types and mark executables.

// 	Controlled by -F, -p, and --indicator-style.  */

// enum indicator_style
//   {
// 	none, /*     --indicator-style=none */
// 	slash, /* -p, --indicator-style=slash */
// 	file_type, /*     --indicator-style=file-type */
// 	classify /* -F, --indicator-style=classify */
//   };

// static enum indicator_style indicator_style;

// /* Names of indicator styles.  */
// static char const *const indicator_style_args[] =
// {
//   "none", "slash", "file-type", "classify", NULL
// };
// static enum indicator_style const indicator_style_types[] =
// {
//   none, slash, file_type, classify
// };
// ARGMATCH_VERIFY (indicator_style_args, indicator_style_types);

// /* True means use colors to mark types.  Also define the different
// 	colors as well as the stuff for the LS_COLORS environment variable.
// 	The LS_COLORS variable is now in a termcap-like format.  */

// static bool print_with_color;

// static bool print_hyperlink;

// /* Whether we used any colors in the output so far.  If so, we will
// 	need to restore the default color later.  If not, we will need to
// 	call prep_non_filename_text before using color for the first time. */

// static bool used_color = false;

// enum when_type
//   {
// 	when_never,  /* 0: default or --color=never */
// 	when_always, /* 1: --color=always */
// 	when_if_tty  /* 2: --color=tty */
//   };

// enum Dereference_symlink
//   {
// 	DEREF_UNDEFINED = 1,
// 	DEREF_NEVER,
// 	DEREF_COMMAND_LINE_ARGUMENTS, /* -H */
// 	DEREF_COMMAND_LINE_SYMLINK_TO_DIR, /* the default, in certain cases */
// 	DEREF_ALWAYS   /* -L */
//   };

// enum indicator_no
//   {
// 	C_LEFT, C_RIGHT, C_END, C_RESET, C_NORM, C_FILE, C_DIR, C_LINK,
// 	C_FIFO, C_SOCK,
// 	C_BLK, C_CHR, C_MISSING, C_ORPHAN, C_EXEC, C_DOOR, C_SETUID, C_SETGID,
// 	C_STICKY, C_OTHER_WRITABLE, C_STICKY_OTHER_WRITABLE, C_CAP, C_MULTIHARDLINK,
// 	C_CLR_TO_EOL
//   };

// static const char *const indicator_name[]=
// {
// 	"lc", "rc", "ec", "rs", "no", "fi", "di", "ln", "pi", "so",
// 	"bd", "cd", "mi", "or", "ex", "do", "su", "sg", "st",
// 	"ow", "tw", "ca", "mh", "cl", NULL
// };

// static struct bin_str color_indicator[] =
// {
// 	{ LEN_STR_PAIR ("\033[") },  /* lc: Left of color sequence */
// 	{ LEN_STR_PAIR ("m") },  /* rc: Right of color sequence */
// 	{ 0, NULL },   /* ec: End color (replaces lc+rs+rc) */
// 	{ LEN_STR_PAIR ("0") },  /* rs: Reset to ordinary colors */
// 	{ 0, NULL },   /* no: Normal */
// 	{ 0, NULL },   /* fi: File: default */
// 	{ LEN_STR_PAIR ("01;34") },  /* di: Directory: bright blue */
// 	{ LEN_STR_PAIR ("01;36") },  /* ln: Symlink: bright cyan */
// 	{ LEN_STR_PAIR ("33") },  /* pi: Pipe: yellow/brown */
// 	{ LEN_STR_PAIR ("01;35") },  /* so: Socket: bright magenta */
// 	{ 0, NULL },   /* mi: Missing file: undefined */
// 	{ 0, NULL },   /* or: Orphaned symlink: undefined */
// 	{ LEN_STR_PAIR ("01;32") },  /* ex: Executable: bright green */
// 	{ LEN_STR_PAIR ("01;35") },  /* do: Door: bright magenta */
// 	{ LEN_STR_PAIR ("37;41") },  /* su: setuid: white on red */
// 	{ LEN_STR_PAIR ("30;43") },  /* sg: setgid: black on yellow */
// 	{ LEN_STR_PAIR ("37;44") },  /* st: sticky: black on blue */
// 	{ LEN_STR_PAIR ("34;42") },  /* ow: other-writable: blue on green */
// 	{ LEN_STR_PAIR ("30;42") },  /* tw: ow w/ sticky: black on green */
// 	{ LEN_STR_PAIR ("30;41") },  /* ca: black on red */
// 	{ 0, NULL },   /* mh: disabled by default */
// 	{ LEN_STR_PAIR ("\033[K") }, /* cl: clear to end of line */
// };

// /* FIXME: comment  */
// static struct color_ext_type *color_ext_list = NULL;

// /* Buffer for color sequences */
// static char *color_buf;

// /* True means to check for orphaned symbolic link, for displaying
// 	colors.  */
// static bool check_symlink_color;

// /* True means mention the inode number of each file.  -i  */
// static bool print_inode;

// /* What to do with symbolic links.  Affected by -d, -F, -H, -l (and
// 	other options that imply -l), and -L.  */
// static enum Dereference_symlink dereference;

// /* True means when a directory is found, display info on its
// 	contents.  -R  */
// static bool recursive;

// /* True means when an argument is a directory name, display info
// 	on it itself.  -d  */
// static bool immediate_dirs;

// /* True means that directories are grouped before files. */
// static bool directories_first;

// /* Which files to ignore.  */

// static enum
// {
//   /* Ignore files whose names start with '.', and files specified by
// 	  --hide and --ignore.  */
//   IGNORE_DEFAULT,

//   /* Ignore '.', '..', and files specified by --ignore.  */
//   IGNORE_DOT_AND_DOTDOT,

//   /* Ignore only files specified by --ignore.  */
//   IGNORE_MINIMAL
// } ignore_mode;

// /* A linked list of shell-style globbing patterns.  If a non-argument
// 	file name matches any of these patterns, it is ignored.
// 	Controlled by -I.  Multiple -I options accumulate.
// 	The -B option adds '*~' and '.*~' to this list.  */

// struct ignore_pattern
//   {
// 	 const char *pattern;
// 	 struct ignore_pattern *next;
//   };

// static struct ignore_pattern *ignore_patterns;

// /* Similar to IGNORE_PATTERNS, except that -a or -A causes this
// 	variable itself to be ignored.  */
// static struct ignore_pattern *hide_patterns;

// /* True means output nongraphic chars in file names as '?'.
// 	(-q, --hide-control-chars)
// 	qmark_funny_chars and the quoting style (-Q, --quoting-style=WORD) are
// 	independent.  The algorithm is: first, obey the quoting style to get a
// 	string representing the file name;  then, if qmark_funny_chars is set,
// 	replace all nonprintable chars in that string with '?'.  It's necessary
// 	to replace nonprintable chars even in quoted strings, because we don't
// 	want to mess up the terminal if control chars get sent to it, and some
// 	quoting methods pass through control chars as-is.  */
// static bool qmark_funny_chars;

// /* Quoting options for file and dir name output.  */

// static struct quoting_options *filename_quoting_options;
// static struct quoting_options *dirname_quoting_options;

// /* The number of chars per hardware tab stop.  Setting this to zero
// 	inhibits the use of TAB characters for separating columns.  -T */
// static size_t tabsize;

// /* True means print each directory name before listing it.  */

// static bool print_dir_name;

// /* The line length to use for breaking lines in many-per-line format.
// 	Can be set with -w.  */

// static size_t line_length;

// /* The local time zone rules, as per the TZ environment variable.  */

// // static timezone_t localtz;

// /* If true, the file listing format requires that stat be called on
// 	each file.  */

// static bool format_needs_stat;

// /* Similar to 'format_needs_stat', but set if only the file type is
// 	needed.  */

// static bool format_needs_type;

// /* An arbitrary limit on the number of bytes in a printed timestamp.
// 	This is set to a relatively small value to avoid the need to worry
// 	about denial-of-service attacks on servers that run "ls" on behalf
// 	of remote clients.  1000 bytes should be enough for any practical
// 	timestamp format.  */

// enum { TIME_STAMP_LEN_MAXIMUM = 1000 };

// /* strftime formats for non-recent and recent files, respectively, in
// 	-l output.  */

// static char const *long_time_format[2] =
// {
// 	 /* strftime format for non-recent files (older than 6 months), in
// 		 -l output.  This should contain the year, month and day (at
// 		 least), in an order that is understood by people in your
// 		 locale's territory.  Please try to keep the number of used
// 		 screen columns small, because many people work in windows with
// 		 only 80 columns.  But make this as wide as the other string
// 		 below, for recent files.  */
// 	 /* TRANSLATORS: ls output needs to be aligned for ease of reading,
// 		 so be wary of using variable width fields from the locale.
// 		 Note %b is handled specially by ls and aligned correctly.
// 		 Note also that specifying a width as in %5b is erroneous as strftime
// 		 will count bytes rather than characters in multibyte locales.  */
// 	 "%b %e  %Y",
// 	 /* strftime format for recent files (younger than 6 months), in -l
// 		 output.  This should contain the month, day and time (at
// 		 least), in an order that is understood by people in your
// 		 locale's territory.  Please try to keep the number of used
// 		 screen columns small, because many people work in windows with
// 		 only 80 columns.  But make this as wide as the other string
// 		 above, for non-recent files.  */
// 	 /* TRANSLATORS: ls output needs to be aligned for ease of reading,
// 		 so be wary of using variable width fields from the locale.
// 		 Note %b is handled specially by ls and aligned correctly.
// 		 Note also that specifying a width as in %5b is erroneous as strftime
// 		 will count bytes rather than characters in multibyte locales.  */
// 	 "%b %e %H:%M"
// };


// /* If nonzero, the value of the pending fatal signal.  */

// // static sig_atomic_t volatile interrupt_signal;

// /* A count of the number of pending stop signals that have been received.  */

// // static sig_atomic_t volatile stop_signal_count;

// /* Desired exit status.  */

// static int exit_status;

// /* Exit statuses.  */
// enum
//   {
// 	 /* "ls" had a minor problem.  E.g., while processing a directory,
// 		 ls obtained the name of an entry via readdir, yet was later
// 		 unable to stat that name.  This happens when listing a directory
// 		 in which entries are actively being removed or renamed.  */
// 	 LS_MINOR_PROBLEM = 1,

// 	 /* "ls" had more serious trouble (e.g., memory exhausted, invalid
// 		 option or failure to stat a command line argument.  */
// 	 LS_FAILURE = 2
//   };

// /* For long options that have no equivalent short option, use a
// 	non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
// enum
// {
//   AUTHOR_OPTION = CHAR_MAX + 1,
//   BLOCK_SIZE_OPTION,
//   COLOR_OPTION,
//   DEREFERENCE_COMMAND_LINE_SYMLINK_TO_DIR_OPTION,
//   FILE_TYPE_INDICATOR_OPTION,
//   FORMAT_OPTION,
//   FULL_TIME_OPTION,
//   GROUP_DIRECTORIES_FIRST_OPTION,
//   HIDE_OPTION,
//   HYPERLINK_OPTION,
//   INDICATOR_STYLE_OPTION,
//   QUOTING_STYLE_OPTION,
//   SHOW_CONTROL_CHARS_OPTION,
//   SI_OPTION,
//   SORT_OPTION,
//   TIME_OPTION,
//   TIME_STYLE_OPTION
// };

// static t_option const long_options[] =
// {
//   {"all", no_argument, NULL, 'a'},
//   {"escape", no_argument, NULL, 'b'},
//   {"directory", no_argument, NULL, 'd'},
//   {"dired", no_argument, NULL, 'D'},
//   {"full-time", no_argument, NULL, FULL_TIME_OPTION},
//   {"group-directories-first", no_argument, NULL,
// 	GROUP_DIRECTORIES_FIRST_OPTION},
//   {"human-readable", no_argument, NULL, 'h'},
//   {"inode", no_argument, NULL, 'i'},
//   {"kibibytes", no_argument, NULL, 'k'},
//   {"numeric-uid-gid", no_argument, NULL, 'n'},
//   {"no-group", no_argument, NULL, 'G'},
//   {"hide-control-chars", no_argument, NULL, 'q'},
//   {"reverse", no_argument, NULL, 'r'},
//   {"size", no_argument, NULL, 's'},
//   {"width", required_argument, NULL, 'w'},
//   {"almost-all", no_argument, NULL, 'A'},
//   {"ignore-backups", no_argument, NULL, 'B'},
//   {"classify", no_argument, NULL, 'F'},
//   {"file-type", no_argument, NULL, FILE_TYPE_INDICATOR_OPTION},
//   {"si", no_argument, NULL, SI_OPTION},
//   {"dereference-command-line", no_argument, NULL, 'H'},
//   {"dereference-command-line-symlink-to-dir", no_argument, NULL,
// 	DEREFERENCE_COMMAND_LINE_SYMLINK_TO_DIR_OPTION},
//   {"hide", required_argument, NULL, HIDE_OPTION},
//   {"ignore", required_argument, NULL, 'I'},
//   {"indicator-style", required_argument, NULL, INDICATOR_STYLE_OPTION},
//   {"dereference", no_argument, NULL, 'L'},
//   {"literal", no_argument, NULL, 'N'},
//   {"quote-name", no_argument, NULL, 'Q'},
//   {"quoting-style", required_argument, NULL, QUOTING_STYLE_OPTION},
//   {"recursive", no_argument, NULL, 'R'},
//   {"format", required_argument, NULL, FORMAT_OPTION},
//   {"show-control-chars", no_argument, NULL, SHOW_CONTROL_CHARS_OPTION},
//   {"sort", required_argument, NULL, SORT_OPTION},
//   {"tabsize", required_argument, NULL, 'T'},
//   {"time", required_argument, NULL, TIME_OPTION},
//   {"time-style", required_argument, NULL, TIME_STYLE_OPTION},
//   {"color", optional_argument, NULL, COLOR_OPTION},
//   {"hyperlink", optional_argument, NULL, HYPERLINK_OPTION},
//   {"block-size", required_argument, NULL, BLOCK_SIZE_OPTION},
//   {"context", no_argument, 0, 'Z'},
//   {"author", no_argument, NULL, AUTHOR_OPTION},
//   {NULL, 0, NULL, 0}
// };

// static char const *const format_args[] =
// {
//   "verbose", "long", "commas", "horizontal", "across",
//   "vertical", "single-column", NULL
// };
// static enum e_format const format_types[] =
// {
//   long_format, long_format, with_commas, horizontal, horizontal,
//   many_per_line, one_per_line
// };


// static char const *const sort_args[] =
// {
//   "none", "time", "size", "extension", "version", NULL
// };
// static enum e_sort_type const sort_types[] =
// {
//   sort_none, sort_time, sort_size, sort_extension, sort_version
// };


// static char const *const time_args[] =
// {
//   "atime", "access", "use", "ctime", "status", NULL
// };
// static enum e_time_type const time_types[] =
// {
//   time_atime, time_atime, time_atime, time_ctime, time_ctime
// };


// static char const *const when_args[] =
// {
//   /* force and none are for compatibility with another color-ls version */
//   "always", "yes", "force",
//   "never", "no", "none",
//   "auto", "tty", "if-tty", NULL
// };
// static enum when_type const when_types[] =
// {
//   when_always, when_always, when_always,
//   when_never, when_never, when_never,
//   when_if_tty, when_if_tty, when_if_tty
// };


// /* Information about filling a column.  */
// struct column_info
// {
//   bool valid_len;
//   size_t line_len;
//   size_t *col_arr;
// };

// /* Array with information about column filledness.  */
// static struct column_info *column_info;

// /* Maximum number of columns ever possible for this display.  */
// static size_t max_idx;

// /* The minimum width of a column is 3: 1 character for the name and 2
// 	for the separating white space.  */
// #define MIN_COLUMN_WIDTH 3


// /* This zero-based index is used solely with the --dired option.
// 	When that option is in effect, this counter is incremented for each
// 	byte of output generated by this program so that the beginning
// 	and ending indices (in that output) of every file name can be recorded
// 	and later output themselves.  */
// static size_t dired_pos;

// static int
// decode_switches (int argc, char **argv)
// {
// 	char *time_style_option = NULL;

// 	bool sort_type_specified = false;
// 	bool kibibytes_specified = false;

// 	qmark_funny_chars = false;

// 	/* This is for the 'ls' program.  */
// 	if (isatty (STDOUT_FILENO))
// 	{
// 		format = many_per_line;
// 		set_quoting_style (NULL, shell_escape_quoting_style);
// 			 /* See description of qmark_funny_chars, above.  */
// 		qmark_funny_chars = true;
// 	}
// 	else
// 	{
// 		format = one_per_line;
// 		qmark_funny_chars = false;
// 	}

// 	time_type = time_mtime;
// 	sort_type = sort_name;
// 	sort_reverse = false;
// 	numeric_ids = false;
// 	print_block_size = false;
// 	indicator_style = none;
// 	print_inode = false;
// 	dereference = DEREF_UNDEFINED;
// 	recursive = false;
// 	immediate_dirs = false;
// 	ignore_mode = IGNORE_DEFAULT;
// 	ignore_patterns = NULL;
// 	hide_patterns = NULL;
// 	print_scontext = false;

// 	getenv_quoting_style ();

// 	line_length = 80;
// 	{
// 		char const *p = getenv ("COLUMNS");
// 		if (p && *p && ! set_line_length (p))
// 			error (0, 0,
// 				 _("ignoring invalid width in environment variable COLUMNS: %s"),
// 				 quote (p));
//   	}

// #ifdef TIOCGWINSZ
//   {
// 	 struct winsize ws;

// 	 if (ioctl (STDOUT_FILENO, TIOCGWINSZ, &ws) != -1

// 		  && 0 < ws.ws_col && ws.ws_col == (size_t) ws.ws_col)
// 		line_length = ws.ws_col;
// }
// #endif

// 	{
// 	 char const *p = getenv ("TABSIZE");
// 	 tabsize = 8;
// 	 if (p)
// 		{
// 		  unsigned long int tmp_ulong;
// 		  if (xstrtoul (p, NULL, 0, &tmp_ulong, NULL) == LONGINT_OK
// 				&& tmp_ulong <= SIZE_MAX)
// 			 {
// 				tabsize = tmp_ulong;
// 			 }
// 		  else
// 			 {
// 				error (0, 0,
// 				 _("ignoring invalid tab size in environment variable TABSIZE: %s"),
// 						 quote (p));
// 			 }
// 		}
// 	}

// 	while (true)
// 	{
// 		int oi = -1;
// 		int c = getopt_long (argc, argv,
// 									"abcdfghiklmnopqrstuvw:xABCDFGHI:LNQRST:UXZ1",
// 									long_options, &oi);
// 		if (c == -1)
// 		  break;

// 		switch (c)
// 		{
// 			case 'a':
// 				ignore_mode = IGNORE_MINIMAL;
// 				break;

// 		//   case 'b':
// 		// 	 set_quoting_style (NULL, escape_quoting_style);
// 		// 	 break;

// 			case 'c':
// 				time_type = time_ctime;
// 				break;

// 			case 'd':
// 				immediate_dirs = true;
// 				break;

// 			case 'f':
// 				/* Same as enabling -a -U and disabling -l -s.  */
// 				ignore_mode = IGNORE_MINIMAL;
// 				sort_type = sort_none;
// 				sort_type_specified = true;
// 				/* disable -l */
// 			 	if (format == long_format)
// 					format = (isatty (STDOUT_FILENO) ? many_per_line : one_per_line);
// 				print_block_size = false; /* disable -s */
// 				print_with_color = false; /* disable --color */
// 				print_hyperlink = false; /* disable --hyperlink */
// 				break;

// 			case FILE_TYPE_INDICATOR_OPTION: /* --file-type */
// 				indicator_style = file_type;
// 				break;

// 			case 'g':
// 				format = long_format;
// 				print_owner = false;
// 				break;

// 			case 'h':
// 				file_human_output_opts = human_output_opts = 16 | 128 | 32;
// 				file_output_block_size = output_block_size = 1;
// 				break;

// 			case 'i':
// 				print_inode = true;
// 				break;

// 			case 'k':
// 				kibibytes_specified = true;
// 				break;

// 			case 'l':
// 				format = long_format;
// 				break;

// 			case 'm':
// 				format = with_commas;
// 				break;

// 			case 'n':
// 				numeric_ids = true;
// 				format = long_format;
// 				break;

// 			case 'o':  /* Just like -l, but don't display group info.  */
// 				format = long_format;
// 				print_group = false;
// 				break;

// 			case 'p':
// 				indicator_style = slash;
// 				break;

// 			case 'q':
// 				qmark_funny_chars = true;
// 				break;

// 			case 'r':
// 				sort_reverse = true;
// 				break;

// 			case 's':
// 				print_block_size = true;
// 				break;

// 			case 't':
// 				sort_type = sort_time;
// 				sort_type_specified = true;
// 				break;

// 			case 'u':
// 				time_type = time_atime;
// 				break;

// 			case 'v':
// 				sort_type = sort_version;
// 				sort_type_specified = true;
// 				break;

// 			case 'w':
// 				if (! set_line_length (optarg))
// 					die (LS_FAILURE, 0, "%s: %s", _("invalid line width"),
// 						quote (optarg));
// 				break;

// 			case 'x':
// 				format = horizontal;
// 				break;

// 			case 'A':
// 				ignore_mode = IGNORE_DOT_AND_DOTDOT;
// 				break;

// 			case 'B':
// 				add_ignore_pattern ("*~");
// 				add_ignore_pattern (".*~");
// 				break;

// 			case 'C':
// 				format = many_per_line;
// 				break;

// 			case 'D':
// 				dired = true;
// 				break;

// 			case 'F':
// 				indicator_style = classify;
// 				break;

// 			case 'G':  /* inhibit display of group info */
// 				print_group = false;
// 				break;

// 			case 'H':
// 				dereference = DEREF_COMMAND_LINE_ARGUMENTS;
// 				break;

// 			case DEREFERENCE_COMMAND_LINE_SYMLINK_TO_DIR_OPTION:
// 				dereference = DEREF_COMMAND_LINE_SYMLINK_TO_DIR;
// 				break;

// 			case 'I':
// 				add_ignore_pattern (g_optarg);
// 				break;

// 			case 'L':
// 				dereference = DEREF_ALWAYS;
// 				break;

// 			case 'N':
// 				set_quoting_style (NULL, literal_quoting_style);
// 				break;

// 			case 'Q':
// 				set_quoting_style (NULL, c_quoting_style);
// 				break;

// 			case 'R':
// 				recursive = true;
// 				break;

// 			case 'S':
// 				sort_type = sort_size;
// 				sort_type_specified = true;
// 				break;

// 			case 'T':
// 				tabsize = xnumtoumax (g_optarg, 0, 0, SIZE_MAX, "",
// 											_("invalid tab size"), LS_FAILURE);
// 				break;

// 			case 'U':
// 				sort_type = sort_none;
// 				sort_type_specified = true;
// 				break;

// 			case 'X':
// 				sort_type = sort_extension;
// 				sort_type_specified = true;
// 				break;

// 			case '1':
// 				/* -1 has no effect after -l.  */
// 				if (format != long_format)
// 					format = one_per_line;
// 				break;

// 			case AUTHOR_OPTION:
// 				print_author = true;
// 				break;

// 			case HIDE_OPTION:
// 				{
// 					struct ignore_pattern *hide = xmalloc (sizeof *hide);
// 					hide->pattern = g_optarg;
// 					hide->next = hide_patterns;
// 					hide_patterns = hide;
// 				}
// 				break;

// 			case SORT_OPTION:
// 				sort_type = XARGMATCH ("--sort", g_optarg, sort_args, sort_types);
// 				sort_type_specified = true;
// 				break;

// 			case GROUP_DIRECTORIES_FIRST_OPTION:
// 				directories_first = true;
// 				break;

// 			case TIME_OPTION:
// 				time_type = XARGMATCH ("--time", g_optarg, time_args, time_types);
// 				break;

// 			case FORMAT_OPTION:
// 				format = XARGMATCH ("--format", g_optarg, format_args, format_types);
// 				break;

// 			case FULL_TIME_OPTION:
// 				format = long_format;
// 				time_style_option = bad_cast ("full-iso");
// 				break;

// 			case COLOR_OPTION:
// 				{
// 					int i;
// 					if (g_optarg)
// 					i = XARGMATCH ("--color", g_optarg, when_args, when_types);
// 					else
// 					/* Using --color with no argument is equivalent to using
// 						--color=always.  */
// 					i = when_always;

// 					print_with_color = (i == when_always
// 											|| (i == when_if_tty
// 													&& isatty (STDOUT_FILENO)));

// 					if (print_with_color)
// 					{
// 						/* Don't use TAB characters in output.  Some terminal
// 							emulators can't handle the combination of tabs and
// 							color codes on the same line.  */
// 						tabsize = 0;
// 					}
// 					break;
// 				}

// 			case HYPERLINK_OPTION:
// 				{
// 					int i;
// 					if (g_optarg)
// 					i = XARGMATCH ("--hyperlink", g_optarg, when_args, when_types);
// 					else
// 					/* Using --hyperlink with no argument is equivalent to using
// 						--hyperlink=always.  */
// 					i = when_always;

// 					print_hyperlink = (i == when_always
// 											|| (i == when_if_tty
// 												&& isatty (STDOUT_FILENO)));
// 					break;
// 				}

// 			case INDICATOR_STYLE_OPTION:
// 				indicator_style = XARGMATCH ("--indicator-style", g_optarg,
// 														indicator_style_args,
// 														indicator_style_types);
// 				break;

// 			case QUOTING_STYLE_OPTION:
// 				set_quoting_style (NULL,
// 										XARGMATCH ("--quoting-style", g_optarg,
// 														quoting_style_args,
// 														quoting_style_vals));
// 				break;

// 			case TIME_STYLE_OPTION:
// 				time_style_option = g_optarg;
// 				break;

// 			case SHOW_CONTROL_CHARS_OPTION:
// 				qmark_funny_chars = false;
// 				break;

// 			case BLOCK_SIZE_OPTION:
// 				{
// 					enum strtol_error e = human_options (g_optarg, &human_output_opts,
// 																	&output_block_size);
// 					if (e != LONGINT_OK)
// 					xstrtol_fatal (e, oi, 0, long_options, g_optarg);
// 					file_human_output_opts = human_output_opts;
// 					file_output_block_size = output_block_size;
// 				}
// 				break;

// 			case SI_OPTION:
// 				file_human_output_opts = human_output_opts =
// 					human_autoscale | human_SI;
// 				file_output_block_size = output_block_size = 1;
// 				break;

// 			case 'Z':
// 				print_scontext = true;
// 				break;

// 			case_GETOPT_HELP_CHAR;

// 			case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

// 			default:
// 				usage (LS_FAILURE);
// 			}
// 	 }

//   if (! output_block_size)
// 	 {
// 		char const *ls_block_size = getenv ("LS_BLOCK_SIZE");
// 		human_options (ls_block_size,
// 							&human_output_opts, &output_block_size);
// 		if (ls_block_size || getenv ("BLOCK_SIZE"))
// 		  {
// 			 file_human_output_opts = human_output_opts;
// 			 file_output_block_size = output_block_size;
// 		  }
// 		if (kibibytes_specified)
// 		  {
// 			 human_output_opts = 0;
// 			 output_block_size = 1024;
// 		  }
// 	 }

//   /* Determine the max possible number of display columns.  */
//   max_idx = line_length / MIN_COLUMN_WIDTH;
//   /* Account for first display column not having a separator,
// 	  or line_lengths shorter than MIN_COLUMN_WIDTH.  */
//   max_idx += line_length % MIN_COLUMN_WIDTH != 0;

//   enum quoting_style qs = get_quoting_style (NULL);
//   align_variable_outer_quotes = format != with_commas
// 										  && format != one_per_line
// 										  && (line_length || format == long_format)
// 										  && (qs == shell_quoting_style
// 												|| qs == shell_escape_quoting_style
// 												|| qs == c_maybe_quoting_style);
//   filename_quoting_options = clone_quoting_options (NULL);
//   if (qs == escape_quoting_style)
// 	 set_char_quoting (filename_quoting_options, ' ', 1);
//   if (file_type <= indicator_style)
// 	 {
// 		char const *p;
// 		for (p = &"*=>@|"[indicator_style - file_type]; *p; p++)
// 		  set_char_quoting (filename_quoting_options, *p, 1);
// 	 }

//   dirname_quoting_options = clone_quoting_options (NULL);
//   set_char_quoting (dirname_quoting_options, ':', 1);

//   /* --dired is meaningful only with --format=long (-l).
// 	  Otherwise, ignore it.  FIXME: warn about this?
// 	  Alternatively, make --dired imply --format=long?  */
//   if (dired && (format != long_format || print_hyperlink))
// 	 dired = false;

//   /* If -c or -u is specified and not -l (or any other option that implies -l),
// 	  and no sort-type was specified, then sort by the ctime (-c) or atime (-u).
// 	  The behavior of ls when using either -c or -u but with neither -l nor -t
// 	  appears to be unspecified by POSIX.  So, with GNU ls, '-u' alone means
// 	  sort by atime (this is the one that's not specified by the POSIX spec),
// 	  -lu means show atime and sort by name, -lut means show atime and sort
// 	  by atime.  */

//   if ((time_type == time_ctime || time_type == time_atime)
// 		&& !sort_type_specified && format != long_format)
// 	 {
// 		sort_type = sort_time;
// 	 }

//   if (format == long_format)
// 	 {
// 		char *style = time_style_option;
// 		static char const posix_prefix[] = "posix-";

// 		if (! style)
// 		  if (! (style = getenv ("TIME_STYLE")))
// 			 style = bad_cast ("locale");

// 		while (STREQ_LEN (style, posix_prefix, sizeof posix_prefix - 1))
// 		  {
// 			 if (! hard_locale (LC_TIME))
// 				return g_optind;
// 			 style += sizeof posix_prefix - 1;
// 		  }

// 		if (*style == '+')
// 		  {
// 			 char *p0 = style + 1;
// 			 char *p1 = strchr (p0, '\n');
// 			 if (! p1)
// 				p1 = p0;
// 			 else
// 				{
// 				  if (strchr (p1 + 1, '\n'))
// 					 die (LS_FAILURE, 0, _("invalid time style format %s"),
// 							quote (p0));
// 				  *p1++ = '\0';
// 				}
// 			 long_time_format[0] = p0;
// 			 long_time_format[1] = p1;
// 		  }
// 		else
// 		  {
// 			 ptrdiff_t res = argmatch (style, time_style_args,
// 												(char const *) time_style_types,
// 												sizeof (*time_style_types));
// 			 if (res < 0)
// 				{
// 				  /* This whole block used to be a simple use of XARGMATCH.
// 					  but that didn't print the "posix-"-prefixed variants or
// 					  the "+"-prefixed format string option upon failure.  */
// 				  argmatch_invalid ("time style", style, res);

// 				  /* The following is a manual expansion of argmatch_valid,
// 					  but with the added "+ ..." description and the [posix-]
// 					  prefixes prepended.  Note that this simplification works
// 					  only because all four existing time_style_types values
// 					  are distinct.  */
// 				  fputs (_("Valid arguments are:\n"), stderr);
// 				  char const *const *p = time_style_args;
// 				  while (*p)
// 					 fprintf (stderr, "  - [posix-]%s\n", *p++);
// 				  fputs (_("  - +FORMAT (e.g., +%H:%M) for a 'date'-style"
// 							  " format\n"), stderr);
// 				  usage (LS_FAILURE);
// 				}
// 			 switch (res)
// 				{
// 				case full_iso_time_style:
// 				  long_time_format[0] = long_time_format[1] ="%Y-%m-%d %H:%M:%S.%N %z";
// 				  break;

// 				case long_iso_time_style:
// 				  long_time_format[0] = long_time_format[1] = "%Y-%m-%d %H:%M";
// 				  break;

// 				case iso_time_style:
// 				  long_time_format[0] = "%Y-%m-%d ";
// 				  long_time_format[1] = "%m-%d %H:%M";
// 				  break;

// 				// case locale_time_style:
// 				//   if (hard_locale (LC_TIME))
// 				//     {
// 				//       for (int i = 0; i < 2; i++)
// 				//         long_time_format[i] =
// 				//           dcgettext (NULL, long_time_format[i], LC_TIME);
// 				//     }
// 				}
// 		  }

// 		abformat_init ();
// 	 }

//   return g_optind;
// }

// int main(int argc, char **argv)
// {
// 	int   i;
// 	t_pending   *thispending;
// 	int   n_files;

// 	exit_status = EXIT_SUCCESS;
// 	print_dir_name = true;
// 	pending_dirs = NULL;

// 	current_time.tv_sec = 0;
// 	current_time.tv_nsec = -1;

// 	i = decode_switches(argc, argv);

// }