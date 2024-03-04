
#include "libft.h"

char *ft_quote_type(char *str, char type)
{
	char *new;
	char *tmp;
	char equote = type == '\'' ? '\"' : '\'';
	size_t i;

	new = ft_calloc(ft_strlen(str) * 2 + 2, sizeof(char));
	if (!new)
		return (NULL);
	tmp = new;
	*tmp++ = type;
	i = 0;
	while (str[i])
	{
		if (str[i] == type)
		{
			*tmp++ = type;
			*tmp++ = equote;
			*tmp++ = type;
			*tmp++ = equote;
		}
		else
			*tmp++ = str[i];
		i++;
	}
	*tmp++ = type;
	*tmp = '\0';
	return (new);
}

char *ft_quote(char *str)
{
	return ft_quote_type(str, '\"');
}