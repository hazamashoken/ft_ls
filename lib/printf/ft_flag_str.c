/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_flag_str.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <tliangso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 23:34:13 by tliangso          #+#    #+#             */
/*   Updated: 2024/03/02 10:25:15 by tliangso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*plus_flag_str(char *str, t_format *fmt)
{
	int		nb;
	char	*fstr;

	if ((fmt->plus || fmt->space) && (fmt->type == 'd' || fmt->type == 'i'))
	{
		nb = ft_atoi(str);
		if (nb < 0)
			return (str);
		fstr = ft_calloc(sizeof(char), 2);
		if (!fstr)
			return (NULL);
		if (fmt->plus)
			fstr[0] = '+';
		else
			fstr[0] = ' ';
		fstr = ft_strjoin_free(fstr, str);
		if (!fstr)
			return (NULL);
		return (fstr);
	}
	return (str);
}

char	*hash_flag_str(char *str, t_format *fmt)
{
	char	*fstr;

	if (fmt->hash && (fmt->type == 'x' || fmt->type == 'X'))
	{
		fstr = ft_calloc(sizeof(char), 3);
		if (!fstr)
			return (NULL);
		fstr[0] = '0';
		if (str[0] != '0')
		{
			fstr[1] = fmt->type;
			fstr = ft_strjoin_free(fstr, str);
			if (!fstr)
				return (NULL);
		}
		else
			free(str);
		return (fstr);
	}
	return (str);
}

char	*format_pcs_str(char *str, t_format *fmt)
{
	char	*fstr;

	if (fmt->dot && fmt->precision == 0)
		fstr = ft_calloc(sizeof(char), 1);
	else
		fstr = ft_substr(str, 0, fmt->precision);
	if (!fstr)
		return (NULL);
	free(str);
	return (fstr);
}

char	*format_pcs_digit(char *str, t_format *fmt)
{
	char	*fstr;

	if (str[0] == '0' && fmt->precision == 0)
	{
		fstr = ft_calloc(sizeof(char), fmt->precision + 1);
		if (!fstr)
			return (NULL);
		free(str);
		return (fstr);
	}
	else if (fmt->precision >= (int)ft_strlen(str))
	{
		fstr = pcs_digit_operation(str, fmt);
		if (!fstr)
			return (NULL);
		free(str);
		return (fstr);
	}
	return (str);
}
