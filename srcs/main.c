#include "libft.h"
#include "ft_getopt.h"
#include "ft_ls.h"

// static t_option const long_options[] =
// 	{
// 		{"all", no_argument, NULL, 'a'},
// 		{"directory", no_argument, NULL, 'd'},
// 		{"human-readable", no_argument, NULL, 'h'},
// 		{"no-group", no_argument, NULL, 'G'},
// 		{"reverse", no_argument, NULL, 'r'},
// 		{"almost-all", no_argument, NULL, 'A'},
// 		{NULL, 0, NULL, 0}};

t_entry *get_file_stat(const char *filename)
{
	printf("filename: %s\n", filename);
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
	// ft_strdup(file->name);
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

int print_date(time_t mtime)
{
	// print the date for ls -l

	// Get current time
	time_t current_time = time(NULL);

	// Calculate time difference
	time_t time_diff = current_time - mtime;

	// Calculate days since epoch
	int days_since_epoch = time_diff / SECONDS_PER_DAY;

	// Calculate year (assuming no leap years for simplicity)
	int year = 1970;
	while (days_since_epoch >= (year % 4 == 0 ? 366 : 365))
	{
		days_since_epoch -= (year % 4 == 0 ? 366 : 365);
		year++;
	}

	// Calculate month
	int month = 0;
	while (days_since_epoch >= 28)
	{
		if (month == 1 && year % 4 == 0)
		{ // Handle February in leap year
			days_since_epoch -= 29;
		}
		else
		{
			days_since_epoch -= (month == 0 || month == 2 || month == 4 || month == 6 || month == 7 || month == 9 || month == 11) ? 31 : 30;
		}
		month++;
	}

	// Calculate day
	int day = days_since_epoch + 1;

	// Calculate hours and minutes
	time_t time_for_day = time_diff % SECONDS_PER_DAY;
	int hours = time_for_day / (60 * 60);
	int minutes = (time_for_day % (60 * 60)) / 60;

	// Define month names
	const char *month_names[] = {
		"Jan", "Feb", "Mar", "Apr", "May", "Jun",
		"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

	// Print formatted information
	// char formatted_time[23]; // Buffer size

	ft_printf("%s %d %d:%d", month_names[month], day, hours, minutes);
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
	print_file_type(file->mode);
	print_perm_bit(file->mode);
	ft_printf(" %d", file->nlink);
	print_user_group_other(file->uid, file->gid);
	ft_printf(" %d ", file->size);
	display_date(file->stat);
	ft_printf(" %s", file->name);
	ft_printf("\n");
}

void print_dir(t_list *files)
{
	t_entry *file;
	while (files)
	{
		file = files->content;
		if (ft_strncmp(file->name, ".", 1) != 0)
			print_file_info(file);
		files = files->next;
	}
}

void recursively_do_shit(const char *base_path)
{
	char path[1024] = "";
	t_list *entry_lst = NULL;
	size_t stblock_total = 0;
	DIR *fd = opendir(base_path);
	t_entry *entry;

	if (!fd)
	{
		perror("opendir");
		return;
	}

	struct dirent *dir = readdir(fd);
	ft_printf("\n%s:\n", base_path);
	while (dir)
	{
		t_stat statbuf;
		t_entry *file;
		printf("base:%s\n", base_path);
		if (ft_strncmp(base_path, ".", 1) == 0)
		{
			ft_strlcpy(path, (char *)base_path, 1024);
			ft_strlcat(path, "/", 1024);
			ft_strlcat(path, dir->d_name, 1024);
		}
		if (lstat(path, &statbuf) == -1)
		{
			perror("lstat");
			return;
		}
		file = get_file_stat(path);
		if (ft_strncmp(file->name, ".", 1) != 0)
		{

			stblock_total += statbuf.st_blocks / 2;
			ft_lstadd_back(&entry_lst, ft_lstnew(file));
		}
		dir = readdir(fd);
	}
	closedir(fd);
	ft_lstsort(&entry_lst, sort_by_lowername);
	// ft_lstsort(&files, sort_by_mtime);

	ft_printf("total %d\n", stblock_total);
	print_dir(entry_lst);
	while (entry_lst)
	{
		entry = entry_lst->content;
		if (ft_strncmp(entry->name, ".", 1) != 0 && ft_strncmp(entry->name, "..", 2) != 0)
		{
			if (S_ISDIR(entry->mode))
			{

				ft_strlcpy(path, (char *)base_path, 1024);
				ft_strlcat(path, "/", 1024);
				ft_strlcat(path, entry->name, 1024);
				recursively_do_shit(path);
			}
		}
		entry_lst = entry_lst->next;
	}
	ft_lstclear(&entry_lst, free);
}

int main(int argc, char *argv[])
{
	(void)argc;

	if (argv[1] == NULL)
		recursively_do_shit(".");
	else
		recursively_do_shit(argv[1]);

	// print the stat
	return 0;
}