/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_basename.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <tliangso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 00:25:30 by tliangso          #+#    #+#             */
/*   Updated: 2024/03/04 00:25:30 by tliangso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char *ft_basename(char *path)
{
	char *base;
    
    base = ft_strrchr(path, '/');
	if (base == NULL)
		return (char *)path;
	return base + 1;
}