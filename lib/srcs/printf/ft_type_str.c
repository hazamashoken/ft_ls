/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_type_str.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <tliangso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 23:35:36 by tliangso          #+#    #+#             */
/*   Updated: 2024/03/02 10:25:15 by tliangso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*c_type_str(int c)
{
	char	*str;

	str = ft_calloc(sizeof(char), 2);
	if (str == NULL)
		return (NULL);
	*str = c;
	return (str);
}

char	*s_type_str(char *s)
{
	char	*str;

	str = ft_strdup(s);
	if (str == NULL)
		return (NULL);
	return (str);
}

char	*p_type_str(size_t p)
{
	char	*str;

	str = get_hexaddr_str(p, 'x');
	if (str == NULL)
		return (NULL);
	return (str);
}

char	*d_type_str(int d)
{
	char	*str;

	str = ft_itoa(d);
	if (str == NULL)
		return (NULL);
	return (str);
}

char	*u_type_str(unsigned int u)
{
	char	*str;

	str = uint_str(u);
	if (str == NULL)
		return (NULL);
	return (str);
}
