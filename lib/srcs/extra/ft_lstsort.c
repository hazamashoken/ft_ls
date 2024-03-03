/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <tliangso@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 22:37:22 by tliangso          #+#    #+#             */
/*   Updated: 2024/02/27 22:37:36 by tliangso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	swap_content(t_list *a, t_list *b)
{
    void	*temp;

    temp = a->content;
    a->content = b->content;
    b->content = temp;
}

void	ft_lstsort(t_list **lst, int (*cmp)())
{
    t_list	*current;
    t_list	*next;

    if (lst == NULL || *lst == NULL || (*lst)->next == NULL)
        return ;
    current = *lst;
    while (current->next != NULL)
    {
        next = current->next;
        while (next != NULL)
        {
            if (cmp(current->content, next->content) > 0)
                swap_content(current, next);
            next = next->next;
        }
        current = current->next;
    }
}