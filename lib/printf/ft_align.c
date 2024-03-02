/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_align.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <tliangso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 23:33:32 by tliangso          #+#    #+#             */
/*   Updated: 2024/03/02 10:25:15 by tliangso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	fill_prefix(char *fstr, int i, int n, t_format *fmt)
{
	while (i < fmt->width - n
		|| (fmt->width == 1 && *fstr == '\0' && fmt->type != 'c'))
	{
		if (fmt->zero && (!fmt->dot || fmt->precision < 0))
			fstr[i] = '0';
		else
			fstr[i] = ' ';
		i++;
	}
	return (i);
}

int	printlen(char *str)
{
	int		n;

	n = ft_strlen(str);
	if (n == 0)
		return (1);
	else
		return (n);
}

char	*align_left(char *str, t_format *fmt)
{
	char	*fstr;
	int		i;
	int		plen;

	i = 0;
	plen = printlen(str);
	fstr = ft_calloc(sizeof(char), fmt->width + 1);
	if (!fstr)
		return (NULL);
	if (plen == 1 && (fmt->dot && fmt->precision == 0)
		&& (*str == '0' || *str == 0))
		fstr[i++] = ' ';
	while (i < fmt->width)
	{
		if (i < plen && (fmt->type == 's' && *str == 0))
			fstr[i] = ' ';
		else if (i < plen)
			fstr[i] = str[i];
		else
			fstr[i] = ' ';
		i++;
	}
	free(str);
	return (fstr);
}

char	*align_right(char *str, t_format *fmt)
{
	char	*fstr;
	int		i;
	int		j;
	int		plen;

	i = 0;
	j = 0;
	fstr = ft_calloc(sizeof(char), fmt->width + 1);
	plen = printlen(str);
	if (!fstr)
		return (NULL);
	if (str && str[0] == '-'
		&& (fmt->zero && (!fmt->dot || fmt->precision < 0)))
	{
		fstr[i++] = str[j++];
		fmt->width++;
	}
	i = fill_prefix(fstr, i, plen, fmt);
	if (plen == 1 && (fmt->dot && fmt->precision == 0)
		&& (*str == '0' || *str == 0))
		fstr[i++] = ' ';
	while (str && j < plen)
		fstr[i++] = str[j++];
	free(str);
	return (fstr);
}
