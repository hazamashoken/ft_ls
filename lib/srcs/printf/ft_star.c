/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_star.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <tliangso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 23:35:31 by tliangso          #+#    #+#             */
/*   Updated: 2024/03/02 10:25:15 by tliangso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	star_flag(char c, t_format *fmt)
{
	int		star;

	if (c != '*')
		return (0);
	if (fmt->dot)
	{
		star = (va_arg(fmt->args, int));
		fmt->precision = star;
	}
	else
	{
		star = (va_arg(fmt->args, int));
		if (star < 0)
		{
			fmt->minus = 1;
			star *= -1;
		}
		fmt->width = star;
	}
	return (1);
}
