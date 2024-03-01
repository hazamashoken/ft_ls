#include "libft.h"
#include "ft_getopt.h"
#include "ft_ls.h"
#include "ht.h"

// static t_option const long_options[] =
// 	{
// 		{"all", no_argument, NULL, 'a'},
// 		{"directory", no_argument, NULL, 'd'},
// 		{"human-readable", no_argument, NULL, 'h'},
// 		{"no-group", no_argument, NULL, 'G'},
// 		{"reverse", no_argument, NULL, 'r'},
// 		{"almost-all", no_argument, NULL, 'A'},
// 		{NULL, 0, NULL, 0}};

static t_ht *g_file_table;

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
		ft_printf("-");
	else if (S_ISDIR(mode))
		ft_printf("d");
	else if (S_ISCHR(mode))
		ft_printf("c");
	else if (S_ISBLK(mode))
		ft_printf("b");
	else if (S_ISFIFO(mode))
		ft_printf("p");
	else if (S_ISLNK(mode))
		ft_printf("l");
	else if (S_ISSOCK(mode))
		ft_printf("s");
	else
		ft_printf("?");
	return 0;
}

int print_perm_bit(mode_t mode)
{
	int i;
	for (i = 8; i >= 0; i--)
	{
		if (mode & (1 << i))
		{
			if (i % 3 == 2)
				ft_printf("r");
			else if (i % 3 == 1)
				ft_printf("w");
			else
				ft_printf("x");
		}
		else
			ft_printf("-");
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
	ft_printf(" %s %s", pwd->pw_name, grp->gr_name);
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
	ft_printf("%d ", file->stat.st_blocks);
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
	char path[1024];
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
		if (is_current_or_parent(dirent->d_name) == 1 \
		|| is_hidden_but_not_current_or_parent(dirent->d_name) == 1)
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
		if (is_current_or_parent(getbasename(entry->name)) == 1 \
		|| is_hidden_but_not_current_or_parent(getbasename(entry->name)) == 1)
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

int main(int argc, char *argv[])
{
	(void)argc;
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