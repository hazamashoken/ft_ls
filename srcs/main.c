#include "libft.h"
#include "ft_getopt.h"
#include "ft_ls.h"
#include "ht.h"
#include "ft_extra.h"
#include "ft_argmatch.h"

static t_ht *g_file_table;
static t_ls_option g_ls_option;

t_entry *get_file_stat(const char *filename)
{
	t_entry *file = malloc(sizeof(t_entry));
	if (file == NULL)
	{
		perror("malloc");
		return NULL;
	}
	t_stat stat;
	if (lstat(filename, &stat) == -1)
	{
		perror("lstat");
		free(file);
		return NULL;
	}
	ft_strlcpy(file->name, (char *)filename, 256);
	file->size = stat.st_size;
	file->mode = stat.st_mode;
	file->atime = stat.st_atime;
	file->mtime = stat.st_mtime;
	file->ctime = stat.st_ctime;
	file->nlink = stat.st_nlink;
	file->uid = stat.st_uid;
	file->gid = stat.st_gid;
	file->dev = stat.st_dev;
	file->ino = stat.st_ino;
	file->blksize = stat.st_blksize;
	file->stat = stat;
	return file;
}

int print_file_type(mode_t mode)
{
	if (S_ISREG(mode))
		write(1, "-", 1);
	else if (S_ISDIR(mode))
		write(1, "d", 1);
	else if (S_ISCHR(mode))
		write(1, "c", 1);
	else if (S_ISBLK(mode))
		write(1, "b", 1);
	else if (S_ISFIFO(mode))
		write(1, "p", 1);
	else if (S_ISLNK(mode))
		write(1, "l", 1);
	else if (S_ISSOCK(mode))
		write(1, "s", 1);
	else
		write(1, "?", 1);
	return 0;
}

int print_perm_bit(mode_t mode)
{
	char permissions[] = {'x', 'w', 'r'};
	int i;
	for (i = 8; i >= 0; i--)
	{
		if (mode & (1 << i))
			write(STDOUT_FILENO, &permissions[i % 3], 1);
		else
			write(STDOUT_FILENO, "-", 1);
	}
	return 0;
}

int print_user_group_other(uid_t uid, gid_t gid)
{
	struct group *grp;
	struct passwd *pwd;
	grp = getgrgid(gid);
	if (grp == NULL)
	{
		perror("getgrgid");
		return 1;
	}
	pwd = getpwuid(uid);
	if (pwd == NULL)
	{
		perror("getpwuid");
		return 1;
	}
	write(STDOUT_FILENO, " ", 1);
	write(STDOUT_FILENO, pwd->pw_name, ft_strlen(pwd->pw_name));
	write(STDOUT_FILENO, " ", 1);
	write(STDOUT_FILENO, grp->gr_name, ft_strlen(grp->gr_name));
	return 0;
}

int sort_by_lowername(t_entry *a, t_entry *b)
{
	char *str1;
	char *str2;
	char c1;
	char c2;

	str1 = a->name;
	str2 = b->name;
	while (*str1 != '\0' && *str2 != '\0')
	{
		c1 = *str1;
		c2 = *str2;
		if (c1 >= 'A' && c1 <= 'Z')
			c1 += 32;
		if (c2 >= 'A' && c2 <= 'Z')
			c2 += 32;
		if (c1 != c2)
			return c1 - c2;
		str1++;
		str2++;
	}
	return (0);
}

int sort_by_mtime(t_entry *a, t_entry *b)
{
	return a->stat.st_mtime < b->stat.st_mtime;
}

void print_file_info(t_entry *file)
{
	ft_printf("%d ", file->stat.st_blocks / 2);
	print_file_type(file->mode);
	print_perm_bit(file->mode);
	ft_printf(" %d", file->nlink);
	print_user_group_other(file->uid, file->gid);
	ft_printf(" %d ", file->size);
	display_date(file->stat);
	ft_printf(" %s", ft_basename(file->name));
	ft_printf("\n");
}

void print_dir(t_list *files)
{
	t_entry *file;
	while (files)
	{
		file = files->content;
		if (ft_strncmp(file->name, ".", 2) != 0 && ft_strncmp(file->name, "..", 3) != 0)
			print_file_info(file);
		files = files->next;
	}
}

int is_current_or_parent(const char *name)
{
	if (ft_strncmp(name, ".", 2) == 0 || ft_strncmp(name, "..", 3) == 0)
		return 1;
	return 0;
}

int is_hidden_but_not_current_or_parent(const char *name)
{
	if (name[0] == '.' && ft_strncmp(name, ".", 2) != 0 && ft_strncmp(name, "..", 3) != 0)
		return 1;
	return 0;
}

void listFilesRecursively(const char *base_path)
{
	char path[PATH_MAX];
	struct dirent *dirent;
	struct stat statbuf;
	t_list *entry_lst = NULL;
	t_list *head = NULL;
	size_t stblock_total = 0;

	t_entry *entry;

	if (ht_get(g_file_table, base_path))
		return;

	DIR *dir = opendir(base_path);
	if (!dir)
	{
		perror(base_path);
		return;
	}
	ht_set(g_file_table, base_path, ft_strdup(base_path));
	while ((dirent = readdir(dir)) != NULL)
	{
		t_stat statbuf;
		t_entry *file;
		if (is_current_or_parent(dirent->d_name) == 1)
			// || is_hidden_but_not_current_or_parent(dirent->d_name) == 0)
			continue;

		ft_bzero(path, 1024);
		ft_strlcpy(path, (char *)base_path, 1024);
		ft_strlcat(path, "/", 1024);
		ft_strlcat(path, dirent->d_name, 1024);
		if (lstat(path, &statbuf) == -1)
		{
			perror(path);
			return;
		}
		file = get_file_stat(path);
		stblock_total += statbuf.st_blocks / 2;
		ft_lstadd_back(&entry_lst, ft_lstnew(file));
	}
	closedir(dir);
	// ft_lstsort(&entry_lst, sort_by_lowername);
	ft_printf("\n%s:\n", base_path);
	ft_printf("total %d\n", stblock_total);
	print_dir(entry_lst);
	head = entry_lst;
	while (head)
	{
		entry = head->content;
		if (is_current_or_parent(ft_basename(entry->name)) == 1)
		// || is_hidden_but_not_current_or_parent(ft_basename(entry->name)) == 1)
		{
			head = head->next;
			continue;
		}

		// ft_bzero(path, 1024);
		// ft_strlcpy(path, (char *)base_path, 1024);
		// ft_strlcat(path, "/", 1024);
		// ft_strlcat(path, entry->name, 1024);
		// printf("path: %s\n", path);
		if (lstat(entry->name, &statbuf) == -1)
		{
			perror(entry->name);
			head = head->next;
			continue;
		}

		if (S_ISDIR(statbuf.st_mode))
		{
			listFilesRecursively(entry->name);
		}
		head = head->next;
	}
	ft_lstclear(&entry_lst, free);
}
static void usage(int status, char *program_name)
{
	if (status != EXIT_SUCCESS)
		ft_printf("Try `%s --help' for more information.\n", program_name);
	else
		ft_printf("%s\n", HELP);
}

int parse_options(int argc, char *argv[])
{
	t_option const long_options[] = {
		{"all", no_argument, NULL, 'a'},
		{"escape", no_argument, NULL, 'b'},
		{"directory", no_argument, NULL, 'd'},
		// {"dired", no_argument, NULL, 'D'},
		// {"full-time", no_argument, NULL, FULL_TIME_OPTION},
		{"group-directories-first", no_argument, NULL,
		 GROUP_DIRECTORIES_FIRST_OPTION},
		// {"human-readable", no_argument, NULL, 'h'},
		{"inode", no_argument, NULL, 'i'},
		// {"kibibytes", no_argument, NULL, 'k'},
		{"numeric-uid-gid", no_argument, NULL, 'n'},
		{"no-group", no_argument, NULL, 'G'},
		// {"hide-control-chars", no_argument, NULL, 'q'},
		{"reverse", no_argument, NULL, 'r'},
		{"size", no_argument, NULL, 's'},
		{"width", required_argument, NULL, 'w'},
		{"almost-all", no_argument, NULL, 'A'},
		// {"ignore-backups", no_argument, NULL, 'B'},
		{"classify", optional_argument, NULL, 'F'},
		{"file-type", no_argument, NULL, FILE_TYPE_INDICATOR_OPTION},
		// {"dereference-command-line", no_argument, NULL, 'H'},
		// {"ignore", required_argument, NULL, 'I'},
		// {"dereference", no_argument, NULL, 'L'},
		{"literal", no_argument, NULL, 'N'},
		{"quote-name", no_argument, NULL, 'Q'},
		{"quoting-style", required_argument, NULL, QUOTING_STYLE_OPTION},
		{"recursive", no_argument, NULL, 'R'},
		{"format", required_argument, NULL, FORMAT_OPTION},
		// {"show-control-chars", no_argument, NULL, SHOW_CONTROL_CHARS_OPTION},
		// {"sort", required_argument, NULL, SORT_OPTION},
		// {"tabsize", required_argument, NULL, 'T'},
		// {"time", required_argument, NULL, TIME_OPTION},
		// {"time-style", required_argument, NULL, TIME_STYLE_OPTION},
		// {"zero", no_argument, NULL, ZERO_OPTION},
		// {"color", optional_argument, NULL, COLOR_OPTION},
		{"hyperlink", optional_argument, NULL, HYPERLINK_OPTION},
		// {"block-size", required_argument, NULL, BLOCK_SIZE_OPTION},
		// {"context", no_argument, 0, 'Z'},
		{"author", no_argument, NULL, AUTHOR_OPTION},
		{"help", no_argument, NULL, HELP_OPTION},
		{NULL, 0, NULL, 0}};

	int opt;
	// init_option(&g_ls_option);             abcdfghiklmnopqrstuvw:xABCDFGHI:LNQRST:UXZ1
	while ((opt = ft_getopt_long(argc, argv, "abcdfgilmnoprstuvw:xACFGNQRSUX1", long_options, NULL)) != -1)
	{
		switch (opt)
		{
		case '?':
			ft_printf("Try `%s --help' for more information.\n", argv[0]);
			exit(2);
		case 'a':
			g_ls_option.ignore_mode = IGNORE_MINIMAL;
			break;

		case 'b':
			g_ls_option.quoting_style_opt = escape_quoting_style;
			break;

		case 'c':
			g_ls_option.time_type = time_ctime;
			break;

		case 'd':
			g_ls_option.immediate_dirs = true;
			break;

		case 'f':
			g_ls_option.ignore_mode = IGNORE_MINIMAL; /* enable -a */
			g_ls_option.sort_opt = sort_none;		  /* enable -U */
			if (g_ls_option.format_opt == long_format)
				g_ls_option.format_opt = -1;	  /* disable -l */
			g_ls_option.print_with_color = false; /* disable --color */
			g_ls_option.print_hyperlink = false;  /* disable --hyperlink */
			g_ls_option.print_block_size = false; /* disable -s */
			break;

		case FILE_TYPE_INDICATOR_OPTION: /* --file-type */
			g_ls_option.indicator_style = file_type;
			break;

		case 'g':
			g_ls_option.format_opt = long_format;
			g_ls_option.print_owner = false;
			break;

		case 'i':
			g_ls_option.print_inode = true;
			break;

		case 'l':
			g_ls_option.format_opt = long_format;
			break;

		case 'm':
			g_ls_option.format_opt = with_commas;
			break;

		case 'n':
			g_ls_option.numeric_ids = true;
			g_ls_option.format_opt = long_format;
			break;

		case 'o': /* Just like -l, but don't display group info.  */
			g_ls_option.format_opt = long_format;
			g_ls_option.print_group = false;
			break;

		case 'p':
			g_ls_option.indicator_style = slash;
			break;

		case 'r':
			g_ls_option.sort_reverse = true;
			break;

		case 's':
			g_ls_option.print_block_size = true;
			break;

		case 't':
			g_ls_option.sort_opt = sort_time;
			break;

		case 'u':
			g_ls_option.time_type = time_atime;
			break;

		case 'v':
			g_ls_option.sort_opt = sort_version;
			break;

		case 'w':
			// g_ls_option.width_opt = decode_line_length(g_optarg);
			// if (g_ls_option.width_opt < 0)
			// 	error(LS_FAILURE, 0, "%s: %s", "invalid line width",
			// 		  quote(g_optarg));
			break;

		case 'x':
			g_ls_option.format_opt = horizontal;
			break;

		case 'A':
			g_ls_option.ignore_mode = IGNORE_DOT_AND_DOTDOT;
			break;

		case 'C':
			g_ls_option.format_opt = many_per_line;
			break;

		case 'F':
		{
			int i;
			if (g_optarg)
				i = XARGMATCH("--classify", g_optarg, when_args, when_types);
			else
				/* Using --classify with no argument is equivalent to using
					--classify=always.  */
				i = when_always;
			if (i == when_always || (i == when_if_tty && isatty (STDOUT_FILENO)))
				g_ls_option.indicator_style = classify;
			break;
		}

		case 'G': /* inhibit display of group info */
			g_ls_option.print_group = false;
			break;

		case 'N':
			g_ls_option.quoting_style_opt = literal_quoting_style;
			break;

		case 'Q':
			g_ls_option.quoting_style_opt = c_quoting_style;
			break;

		case 'R':
			g_ls_option.recursive = true;
			break;

		case 'S':
			g_ls_option.sort_opt = sort_size;
			break;

		case 'U':
			g_ls_option.sort_opt = sort_none;
			break;

		case 'X':
			g_ls_option.sort_opt = sort_extension;
			break;

		case '1':
			/* -1 has no effect after -l.  */
			if (g_ls_option.format_opt != long_format)
				g_ls_option.format_opt = one_per_line;
			break;

		case AUTHOR_OPTION:
			g_ls_option.print_author = true;
			break;

			// case HIDE_OPTION:
			// {
			// 	struct ignore_pattern *hide = xmalloc(sizeof *hide);
			// 	hide->pattern = g_optarg;
			// 	hide->next = hide_patterns;
			// 	hide_patterns = hide;
			// }
			// break;

			case SORT_OPTION:
				g_ls_option.sort_opt = XARGMATCH("--sort", g_optarg, sort_args, sort_types);
				break;

			case GROUP_DIRECTORIES_FIRST_OPTION:
				g_ls_option.directories_first = true;
				break;

			case FORMAT_OPTION:
				g_ls_option.format_opt = XARGMATCH("--format", g_optarg, format_args,
												   format_types);
				break;

			// case COLOR_OPTION:
			// {
			// 	int i;
			// 	if (optarg)
			// 		i = XARGMATCH("--color", optarg, when_args, when_types);
			// 	else
			// 		/* Using --color with no argument is equivalent to using
			// 			--color=always.  */
			// 		i = when_always;

			// 	g_ls_option.print_with_color = (i == when_always || (i == when_if_tty && stdout_isatty()));
			// 	break;
			// }

			// case HYPERLINK_OPTION:
			// {
			// 	int i;
			// 	if (optarg)
			// 		i = XARGMATCH("--hyperlink", g_optarg, when_args, when_types);
			// 	else
			// 		/* Using --hyperlink with no argument is equivalent to using
			// 			--hyperlink=always.  */
			// 		i = when_always;

			// 	g_ls_option.print_hyperlink = (i == when_always || (i == when_if_tty && stdout_isatty()));
			// 	break;
			// }

			// case INDICATOR_STYLE_OPTION:
			// 	g_ls_option.indicator_style = XARGMATCH("--indicator-style", g_optarg,
			// 											indicator_style_args,
			// 											indicator_style_types);
			// 	break;

			case QUOTING_STYLE_OPTION:
				g_ls_option.quoting_style_opt = XARGMATCH("--quoting-style", g_optarg,
														  quoting_style_args,
														  quoting_style_vals);
				break;

			// case TIME_STYLE_OPTION:
			// 	g_ls_option.time_style_option = g_optarg;
			// 	break;

			// case SHOW_CONTROL_CHARS_OPTION:
			// 	g_ls_option.hide_control_chars_opt = false;
			// 	break;

		case HELP_OPTION:
			ft_printf("%s\n", HELP);
			exit(0);
		default:
			usage(2, argv[0]);
			exit(2);
		}
	}
	return 0;
}

int main(int argc, char *argv[])
{
	(void)argc;

	parse_options(argc, argv);

	g_file_table = ht_create();

	if (argv[1] == NULL)
		listFilesRecursively(".");
	else
		listFilesRecursively(argv[1]);

	// DEBUG remove later
	// t_hti it = ht_iterator(g_file_table);
	// while (ht_next(&it))
	// {
	// 	ft_printf("dir: %s\n", it.value);
	// }

	ht_destory(g_file_table, free);
	return 0;
}