/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_checker.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <tliangso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 23:33:58 by tliangso          #+#    #+#             */
/*   Updated: 2024/03/03 21:44:59 by tliangso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*types_checker(t_format *fmt)
{
	char	*str;

	if (fmt->type == 'c')
		str = c_type_str(va_arg(fmt->args, int));
	else if (fmt->type == 's')
		str = s_type_str(va_arg(fmt->args, char *));
	else if (fmt->type == 'p')
		str = p_type_str((size_t)va_arg(fmt->args, void *));
	else if (fmt->type == 'd' || fmt->type == 'i')
		str = d_type_str(va_arg(fmt->args, int));
	else if (fmt->type == 'u')
		str = u_type_str(va_arg(fmt->args, unsigned int));
	else if (fmt->type == 'x')
		str = x_type_str(va_arg(fmt->args, unsigned int), 'x');
	else if (fmt->type == 'X')
		str = x_type_str(va_arg(fmt->args, unsigned int), 'X');
	else if (fmt->type == '%')
		str = c_type_str('%');
	else
		str = ft_calloc(sizeof(char), 1);
	if (!str)
		return (NULL);
	return (str);
}

char	*flags_checker(char *str, t_format *fmt)
{
	str = plus_flag_str(str, fmt);
	if (!str)
		return (NULL);
	str = hash_flag_str(str, fmt);
	if (!str)
		return (NULL);
	return (str);
}

char	*precision_checker(char *str, t_format *fmt)
{
	char	*fstr;

	if (!fmt->dot)
		return (str);
	if (fmt->type == 's' || fmt->type == 'c')
		fstr = format_pcs_str(str, fmt);
	else
		fstr = format_pcs_digit(str, fmt);
	if (!fstr)
		return (NULL);
	return (fstr);
}

char	*width_checker(char *str, t_format *fmt)
{
	char	*fstr;

	if (fmt->width <= (int)ft_strlen(str))
		return (str);
	if (fmt->minus)
		fstr = align_left(str, fmt);
	else
		fstr = align_right(str, fmt);
	if (!fstr)
		return (NULL);
	return (fstr);
}

void	print_format(t_format *fmt)
{
	char	*str;

	str = types_checker(fmt);
	if (!str)
		str = ft_strdup(NULL_STR);
	if ((!*str && fmt->width == 1 && fmt->dot && !fmt->precision)
		|| (*str == '0' && fmt->width == 1 && fmt->dot && !fmt->precision))
	{
		free(str);
		str = ft_strdup(" ");
	}
	else
	{
		str = flags_checker(str, fmt);
		str = precision_checker(str, fmt);
		str = width_checker(str, fmt);
	}
	if (fmt->type == 'c')
		fmt->total_len += print_char(fmt->fd, str, fmt->width);
	else if (fmt->type == 's' && !str)
		fmt->total_len += printf_empty(str, fmt);
	else
		fmt->total_len += print_str(str, fmt);
	free(str);
}
