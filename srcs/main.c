#include "libft.h"
#include "ft_getopt.h"
#include "ft_ls.h"
#include "ht.h"


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

char *getbasename(char *path)
{
	char *base = ft_strrchr(path, '/');
	if (base == NULL)
		return (char *)path;
	return base + 1;
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
	ft_printf(" %s", getbasename(file->name));
	ft_printf("\n");
}

void print_dir(t_list *files)
{
	t_entry *file;
	while (files)
	{
		file = files->content;
		if (ft_strncmp(file->name, ".", 2) != 0 \
		&& ft_strncmp(file->name, "..", 3) != 0)
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
	if (name[0] == '.' && ft_strncmp(name, ".", 2) != 0 \
	&& ft_strncmp(name, "..", 3) != 0)
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
		return ;


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
		if (is_current_or_parent(dirent->d_name) == 1 )
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
		if (is_current_or_parent(getbasename(entry->name)) == 1 )
		// || is_hidden_but_not_current_or_parent(getbasename(entry->name)) == 1)
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

void	init_option(t_ls_option *option)
{
	option->l = false;
	option->R = false;
	option->a = false;
	option->r = false;
	option->t = false;
	option->u = false;
	option->f = false;
	option->g = false;
	option->d = false;
	option->A = false;
}

int parse_options(int argc, char *argv[])
{
	t_option const long_options[] = {
		{"all", no_argument, NULL, 'a'},
		{"directory", no_argument, NULL, 'd'},
		{"no-group", no_argument, NULL, 'G'},
		{"reverse", no_argument, NULL, 'r'},
		{"almost-all", no_argument, NULL, 'A'},
		{"help", no_argument, NULL, 200},
		{NULL, 0, NULL, 0}};

	int opt;
	init_option(&g_ls_option);
	while ((opt = ft_getopt_long(argc, argv, "lRartufgdAG", long_options, NULL)) != -1)
	{
		switch (opt)
		{
		case 'l':
			ft_printf("option -ln");
			g_ls_option.a = true;
			break;
		case 'R':
			ft_printf("option -R\n");
			g_ls_option.d = true;
			break;
		case 'a':
			ft_printf("option -a\n");
			g_ls_option.a = true;
			break;
		case 'r':
			ft_printf("option -r\n");
			g_ls_option.r = true;
			break;
		case 't':
			ft_printf("option -t\n");
			g_ls_option.t = true;
			break;
		case 'u':
			ft_printf("option -u\n");
			g_ls_option.u = true;
			break;
		case 'f':
			ft_printf("option -f\n");
			g_ls_option.f = true;
			break;
		case 'g':
			ft_printf("option -g\n");
			g_ls_option.g = true;
			break;
		case 'd':
			ft_printf("option -d\n");
			g_ls_option.d = true;
			break;
		case 'A':
			ft_printf("option -A\n");
			g_ls_option.A = true;
			break;
		case '?':
			ft_printf("Try `%s --help' for more information.\n", argv[0]);
			exit (2);
		case 200:
			ft_printf("option --help\n");
			exit (0);
		default:
			ft_printf("Error WTF: %d ??\n", opt);
			exit (2);
		}
	}
	return 0;
}


int main(int argc, char *argv[])
{
	(void)argc;

	parse_options(argc, argv);

	printf("l: %d\n", g_ls_option.l);
	printf("R: %d\n", g_ls_option.R);
	printf("a: %d\n", g_ls_option.a);
	printf("r: %d\n", g_ls_option.r);
	printf("t: %d\n", g_ls_option.t);
	printf("u: %d\n", g_ls_option.u);
	printf("f: %d\n", g_ls_option.f);
	printf("g: %d\n", g_ls_option.g);
	printf("d: %d\n", g_ls_option.d);
	printf("A: %d\n", g_ls_option.A);




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