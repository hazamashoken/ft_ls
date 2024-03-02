/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <tliangso@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/21 14:10:18 by tliangso          #+#    #+#             */
/*   Updated: 2024/03/02 01:41:10 by tliangso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_print_str(char *str, char c, int mode)
{
	int	len;

	len = -1;
	if (!mode && !str)
		return (write(STDOUT_FILENO, "(null)", 6));
	if (mode)
		return (write(STDOUT_FILENO, &c, 1));
	while (str[++len])
		;
	return (write(STDOUT_FILENO, str, len));
}

static void	ft_putnbr(int64_t n, int base, char *charset, int *len)
{
	if (n < 0)
	{
		*len += write(1, "-", 1);
		n = -n;
	}
	if (n >= base)
		ft_putnbr(n / base, base, charset, len);
	*len += write(STDOUT_FILENO, &charset[n % base], 1);
}

static void	ft_putptr(uintptr_t n, uintptr_t base, char *charset, int *len)
{
	if (n >= base)
		ft_putptr(n / base, base, charset, len);
	*len += write(STDOUT_FILENO, &charset[n % base], 1);
}

static int	eval_format(const char *format, va_list ap, int *len, int i)
{
	if (format[i] == 'c')
		*len += ft_print_str(NULL, va_arg(ap, int), 1);
	else if (format[i] == 's')
		*len += ft_print_str(va_arg(ap, char *), 0, 0);
	else if (format[i] == 'd' || format[i] == 'i')
		ft_putnbr(va_arg(ap, int), 10, "0123456789", len);
	else if (format[i] == 'x')
		ft_putnbr(va_arg(ap, unsigned int), 16, "0123456789abcdef", len);
	else if (format[i] == 'X')
		ft_putnbr(va_arg(ap, unsigned int), 16, "0123456789ABCDEF", len);
	else if (format[i] == 'u')
		ft_putnbr(va_arg(ap, unsigned int), 10, "0123456789", len);
	else if (format[i] == 'p')
	{
		*len += write(STDOUT_FILENO, "0x", 2);
		ft_putptr(va_arg(ap, uintptr_t), 16, "0123456789abcdef", len);
	}
	else if (format[i] == '%')
		*len += write(STDOUT_FILENO, "%%", 1);
	else
		return (0);
	return (1);
}

/*
* ft_printf
*/
int	ft_printf(const char *format, ...)
{
	va_list		ap;
	int			i;
	int			len;

	i = 0;
	len = 0;
	va_start(ap, format);
	while (format[i])
	{
		if (format[i] == '%')
		{
			i += eval_format(format, ap, &len, i + 1);
		}
		else
			len += write(STDOUT_FILENO, &format[i], 1);
		i++;
	}
	va_end(ap);
	return (len);
}
