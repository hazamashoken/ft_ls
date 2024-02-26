/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <tliangso@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/21 14:10:18 by tliangso          #+#    #+#             */
/*   Updated: 2024/02/26 23:20:49 by tliangso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_print_str(char *str, char c, int mode, int fd)
{
	int	len;

	len = -1;
	if (!str)
		return (write(fd, "(null)", 6));
	if (mode)
		return (write(fd, &c, 1));
	while (str[++len])
		;
	return (write(fd, str, len));
}

static void	ft_putnbr(int64_t n, int base, char *charset, int *len, int fd)
{
	if (n < 0)
	{
		*len += write(1, "-", 1);
		n = -n;
	}
	if (n >= base)
		ft_putnbr(n / base, base, charset, len, fd);
	*len += write(fd, &charset[n % base], 1);
}

static void	ft_putptr(uintptr_t n, uintptr_t base, char *charset, int *len, int fd)
{
	if (n >= base)
		ft_putptr(n / base, base, charset, len, fd);
	*len += write(fd, &charset[n % base], 1);
}

static int	eval_format(const char *format, va_list ap, int *len, int i, int fd)
{
	if (format[i] == 'c')
		*len += ft_print_str(NULL, va_arg(ap, int), 1, fd);
	else if (format[i] == 's')
		*len += ft_print_str(va_arg(ap, char *), 0, 0, fd);
	else if (format[i] == 'd' || format[i] == 'i')
		ft_putnbr(va_arg(ap, int), 10, "0123456789", len, fd);
	else if (format[i] == 'x')
		ft_putnbr(va_arg(ap, unsigned int), 16, "0123456789abcdef", len, fd);
	else if (format[i] == 'X')
		ft_putnbr(va_arg(ap, unsigned int), 16, "0123456789ABCDEF", len, fd);
	else if (format[i] == 'u')
		ft_putnbr(va_arg(ap, unsigned int), 10, "0123456789", len, fd);
	else if (format[i] == 'p')
	{
		*len += write(fd, "0x", 2);
		ft_putptr(va_arg(ap, uintptr_t), 16, "0123456789abcdef", len, fd);
	}
	else if (format[i] == '%')
		*len += write(fd, "%%", 1);
	else
		return (0);
	return (1);
}

/*
* ft_printf
*/
int	ft_printf(int fd, const char *format, ...)
{
	va_list		ap;
	int			i;
	int			len;

	if (fd < 0)
		return (-1);
	i = 0;
	len = 0;
	va_start(ap, format);
	while (format[i])
	{
		if (format[i] == '%')
		{
			i += eval_format(format, ap, &len, i + 1, fd);
		}
		else
			len += write(fd, &format[i], 1);
		i++;
	}
	va_end(ap);
	return (len);
}
