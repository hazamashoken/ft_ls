/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <tliangso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/09 10:52:47 by tliangso          #+#    #+#             */
/*   Updated: 2024/02/17 14:08:56 by tliangso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// DESCRIPTION
//        The  bzero()  function  erases  the data in the n bytes of the
//        memory starting at the location pointed to by  s,  by  writing
//        zeros (bytes containing '\0') to that area.
//
//        The  explicit_bzero()  function  performs  the  same  task  as
//        bzero().  It differs from bzero() in that it  guarantees  that
//        compiler  optimizations will not remove the erase operation if
//        the compiler deduces that the operation is "unnecessary".
//
// RETURN VALUE
//        None.
#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	ft_memset(s, 0, n);
}
