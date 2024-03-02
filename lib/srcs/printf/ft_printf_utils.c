/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <tliangso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 23:35:22 by tliangso          #+#    #+#             */
/*   Updated: 2024/03/02 10:40:47 by tliangso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_istype(char c)
{
	size_t	i;

	i = 0;
	while (FORMAT[i])
	{
		if (FORMAT[i] == c)
			return (1);
		i++;
	}
	return (0);
}
