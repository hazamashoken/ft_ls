#include <stdio.h>
#include <libgen.h>
#include "ft_printf.h"

char *getbasename(char *path)
{
	char *base = ft_strrchr(path, '/');
	if (base == NULL)
		return (char *)path;
	return base + 1;
}

# define HYPERLINK(path) ft_printf("\e]8;;%s\e\\%s\e]8;;\e\\\n", path, basename(path));


int main(void)
{
    HYPERLINK("/home/tliangso/Desktop/42/advance_core/ft_ls/srcs")
    return 0;
}