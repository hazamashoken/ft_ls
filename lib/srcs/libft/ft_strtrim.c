/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <tliangso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/10 12:00:10 by tliangso          #+#    #+#             */
/*   Updated: 2024/02/17 14:09:17 by tliangso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// PARAMETERS
//    s1: The string to be trimmed.
//    set: The reference set of characters to trim.

// DESCRIPTION
//    Allocates (with malloc(3)) and returns a copy of
//    ’s1’ with the characters specified in ’set’ removed
//    from the beginning and the end of the string.

// RETURN VALUE
//    The trimmed string.
//    NULL if the allocation fails.

#include "libft.h"

static int	check_charset(char c, char const *set)
{
	size_t	i;

	i = 0;
	while (*(set + i))
		if (*(set + i++) == c)
			return (1);
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	start;
	size_t	end;
	char	*str;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	end = ft_strlen(s1);
	while (*(s1 + start) && check_charset(*(s1 + start), set))
		start++;
	while (end > start && check_charset(*(s1 + end - 1), set))
		end--;
	str = malloc(sizeof(char) * (end - start + 1));
	if (str == NULL)
		return (NULL);
	i = 0;
	while (start < end)
		*(str + i++) = *(s1 + start++);
	*(str + i) = '\0';
	return (str);
}
