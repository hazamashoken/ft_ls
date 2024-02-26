/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <tliangso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 20:14:30 by tliangso          #+#    #+#             */
/*   Updated: 2024/02/17 14:09:17 by tliangso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// PARAMETERS
// lst: The address of a pointer to a node.
// del: The address of the function used to delete
// the content of the node.

// DESCRIPTION
// Deletes and frees the given node and every
// successor of that node, using the function ’del’
// and free(3).
// Finally, the pointer to the list must be set to
// NULL.

// RETURN VALUE
// None

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*cur;
	t_list	*next;

	if (lst == NULL || del == NULL)
		return ;
	cur = *lst;
	while (cur != NULL)
	{
		next = cur->next;
		(*del)(cur->content);
		free(cur);
		cur = next;
	}
	*lst = NULL;
}
