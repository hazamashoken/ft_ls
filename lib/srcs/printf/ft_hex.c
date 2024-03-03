/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hex.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <tliangso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 23:34:23 by tliangso          #+#    #+#             */
/*   Updated: 2024/03/03 22:29:22 by tliangso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static size_t	ft_intlen(size_t nb, int div)
{
	int		i;
	size_t	number;

	i = 0;
	if (!nb)
		return (1);
	number = nb;
	while (number)
	{
		number /= div;
		i++;
	}
	return (i);
}

size_t	ft_uintlen(unsigned int nb, int div)
{
	int				i;
	unsigned int	number;

	i = 0;
	if (!nb)
		return (1);
	number = nb;
	while (number)
	{
		number /= div;
		i++;
	}
	return (i);
}

char	*get_hexaddr_str(size_t x, char format)
{
	char	*hex;
	char	*res;
	size_t	len;
	char	*res2;

#if __linux
	if (x == 0)
		return (ft_strdup("(nil)"));
#endif
	if (format == 'x')
		hex = "0123456789abcdef";
	else
		hex = "0123456789ABCDEF";
	len = ft_intlen(x, 16);
	res = (char *)ft_calloc(sizeof(char), len + 1);
	if (res == NULL)
		return (0);
	while (x >= 16)
	{
		*(res + len - 1) = hex[x % 16];
		x /= 16;
		len--;
	}
	*(res) = hex[x];
	res2 = ft_strjoin("0x", res);
	free(res);
	return (res2);
}

char	*get_hex_str(unsigned int x, char format)
{
	char	*hex;
	char	*res;
	int		len;

	if (format == 'x')
		hex = "0123456789abcdef";
	else
		hex = "0123456789ABCDEF";
	len = ft_uintlen(x, 16);
	res = (char *)ft_calloc(sizeof(char), len + 1);
	if (res == NULL)
		return (0);
	while (x >= 16)
	{
		*(res + len - 1) = hex[x % 16];
		x /= 16;
		len--;
	}
	*(res) = hex[x];
	return (res);
}
