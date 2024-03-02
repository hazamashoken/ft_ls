/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_precision_digit.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <tliangso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 23:34:55 by tliangso          #+#    #+#             */
/*   Updated: 2024/03/02 10:25:15 by tliangso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static char	*pcs_digit_fill(char *fstr, char *str, t_format *fmt)
{
	int		i;
	int		j;
	int		len;

	len = ft_strlen(str);
	i = 0;
	j = 0;
	if (str[0] == '-')
	{
		i++;
		j++;
		len--;
		fmt->precision++;
	}
	while (i < fmt->precision)
	{
		if (i < fmt->precision - len)
			fstr[i] = '0';
		else
			fstr[i] = str[j++];
		i++;
	}
	return (fstr);
}

char	*pcs_digit_operation(char *str, t_format *fmt)
{
	char	*fstr;

	if (str[0] == '-')
	{
		fstr = ft_calloc(sizeof(char), fmt->precision + 2);
		if (!fstr)
			return (NULL);
		fstr[0] = '-';
	}
	else
	{
		fstr = ft_calloc(sizeof(char), fmt->precision + 1);
		if (!fstr)
			return (NULL);
	}
	pcs_digit_fill(fstr, str, fmt);
	return (fstr);
}
