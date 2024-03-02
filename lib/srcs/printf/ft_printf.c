/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <tliangso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 23:35:12 by tliangso          #+#    #+#             */
/*   Updated: 2024/03/02 10:25:15 by tliangso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

t_format	*reset_format(t_format *fmt)
{
	fmt->width = 0;
	fmt->precision = 0;
	fmt->zero = 0;
	fmt->dot = 0;
	fmt->minus = 0;
	fmt->plus = 0;
	fmt->is_zero = 0;
	fmt->percent = 0;
	fmt->space = 0;
	fmt->hash = 0;
	fmt->type = 0;
	return (fmt);
}

int	check_format(char c, t_format *fmt)
{
	if (star_flag(c, fmt))
		return (1);
	else if (fmt->dot && ft_isdigit(c))
		fmt->precision = (fmt->precision * 10) + (c - '0');
	else if (ft_istype(c))
		fmt->type = c;
	else if (c == '.')
		fmt->dot = 1;
	else if (c == '+')
		fmt->plus = 1;
	else if (c == '-')
		fmt->minus = 1;
	else if (c == ' ')
		fmt->space = 1;
	else if (c == '#')
		fmt->hash = 1;
	else if (c == '0' && fmt->width == 0)
		fmt->zero = 1;
	else if (ft_isdigit(c))
		fmt->width = (fmt->width * 10) + (c - '0');
	else
		return (0);
	return (1);
}

char	*current_format(t_format *fmt, const char *format, int pos)
{
	char	*current_format;
	int		format_len;
	int		i;

	i = 0;
	format_len = 0;
	while (fmt->type == 0 && check_format(*(format + pos + format_len), fmt))
		format_len++;
	if (fmt->type)
		current_format = (char *)malloc(format_len + 2);
	else
	{
		reset_format(fmt);
		return (NULL);
	}
	if (current_format == NULL)
		return (NULL);
	while (i < format_len + 1)
	{
		*(current_format + i) = *(format + pos + i - 1);
		i++;
	}
	*(current_format + i) = '\0';
	return (current_format);
}

int	ft_eval_format(t_format *fmt, const char *format, int pos)
{
	char	*format_str;
	int		len;

	len = 0;
	format_str = current_format(fmt, format, pos);
	if (format_str)
	{
		print_format(fmt);
		len += ft_strlen(format_str);
		reset_format(fmt);
		free(format_str);
	}
	else
		len = 1;
	return (len - 1);
}

int	ft_printf(const char *format, ...)
{
	int			i;
	int			ret;
	t_format	*tab;

	tab = (t_format *)ft_calloc(sizeof(t_format), 1);
	if (!tab)
		return (-1);
	tab = reset_format(tab);
	tab->total_len = 0;
	va_start(tab->args, format);
	i = -1;
	ret = 0;
	while (*(format + ++i))
	{
		if (*(format + i) == '%')
			i += ft_eval_format(tab, format, i + 1);
		else
			ret += write(1, format + i, 1);
	}
	va_end(tab->args);
	ret += tab->total_len;
	free (tab);
	return (ret);
}
