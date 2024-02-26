# include "libft.h"

int main(int argc, char **argv)
{

    (void)argc;
    (void)argv;
    char *line;

    line = get_next_line(0);
    while(line)
    {
        ft_printf("%s", line);
        free(line);
        line = get_next_line(0);
    }
    return (0);
}