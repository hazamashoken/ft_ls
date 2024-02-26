/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <tliangso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 20:23:07 by tliangso          #+#    #+#             */
/*   Updated: 2024/02/17 14:09:17 by tliangso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// PARAMETERS
// lst: The address of a pointer to a node.
// f: The address of the function used to iterate on
// the list.

// DESCRIPTION
// Iterates the list ’lst’ and applies the function
// ’f’ on the content of each node.

// RETURN VALUE
// None

#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	if (lst == NULL)
		return ;
	(*f)(lst->content);
	ft_lstiter(lst->next, f);
}
