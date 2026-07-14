/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_sort.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 21:11:18 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/16 20:26:30 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_list.h"

int	ft_list_size(t_list *begin_list)
{
	int	i;

	i = 0;
	while (begin_list)
	{
		begin_list = begin_list->next;
		i++;
	}
	return (i);
}

t_list	*ft_swap_lst(t_list *lst1, t_list *lst2)
{
	t_list	*tmp;

	tmp = lst2->next;
	lst2->next = lst1;
	lst1->next = tmp;
	return (lst2);
}

void	ft_list_sort(t_list **begin_list, int (*cmp)())
{
	int		lst_size;
	int		i;
	int		j;
	t_list	**head;

	lst_size = ft_list_size(*begin_list);
	i = 0;
	while (i < lst_size)
	{
		j = 0;
		head = begin_list;
		while (j < lst_size - 1 - i)
		{
			if ((*head) && (*head)->next && (*cmp)((*head)->data,
				(*head)->next->data) > 0)
				*head = ft_swap_lst((*head), (*head)->next);
			head = &(*head)->next;
			j++;
		}
		i++;
	}
}
