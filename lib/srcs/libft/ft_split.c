/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <tliangso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/10 12:13:19 by tliangso          #+#    #+#             */
/*   Updated: 2024/02/17 14:09:17 by tliangso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// PARAMETERS
//    s: The string to be split.
//    c: The delimiter character.

// DESCRIPTION
//    Allocates (with malloc(3)) and returns an array
//    of strings obtained by splitting ’s’ using the
//    character ’c’ as a delimiter. The array must end
//    with a NULL pointer.

// RETURN VALUE
//    The array of new strings resulting from the split.
//    NULL if the allocation fails.

#include "libft.h"

static int	check_separator(char s, char c)
{
	return (s == c);
}

static size_t	count_string(const char *str, char c)
{
	size_t	i;
	size_t	count;

	count = 0;
	i = 0;
	while (*(str + i) != '\0')
	{
		while (*(str + i) != '\0' && *(str + i) == c)
			i++;
		if (*(str + i) != '\0')
			count++;
		while (*(str + i) != '\0' && *(str + i) != c)
			i++;
	}
	return (count);
}

static size_t	ft_strlen_sep(const char *str, char c)
{
	size_t	i;

	i = 0;
	while (*(str + i) != '\0' \
	&& check_separator(*(str + i), c) == 0)
		i++;
	return (i);
}

static char	*word(const char *str, char c)
{
	size_t	word_len;
	size_t	i;
	char	*word;

	i = 0;
	word_len = ft_strlen_sep(str, c);
	word = (char *)malloc(sizeof(char) * word_len + 1);
	if (word == NULL)
		return (NULL);
	while (i < word_len)
	{
		*(word + i) = *(str + i);
		i++;
	}
	*(word + i) = '\0';
	return (word);
}

char	**ft_split(char const *s, char c)
{
	char	**strings;
	size_t	i;

	i = 0;
	if (s == (void *)0)
		return (NULL);
	strings = (char **)malloc(sizeof(char *) * (count_string(s, c) + 1));
	if (strings == NULL)
		return (NULL);
	while (*s != '\0')
	{
		while (*s != '\0' && check_separator(*s, c))
			s++;
		if (*s != '\0')
			*(strings + i++) = word(s, c);
		while (*s != '\0' && check_separator(*s, c) == 0)
			s++;
	}
	*(strings + i) = 0;
	return (strings);
}
