/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sorted_list_insert.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 20:53:41 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/17 14:24:32 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_list.h"

void	ft_sorted_list_insert(t_list **begin_list, void *data, int (*cmp)())
{
	t_list	*head;
	t_list	*new;

	head = *begin_list;
	if (!head || (*cmp)(head->data, data) >= 0)
	{
		*begin_list = ft_create_elem(data);
		(*begin_list)->next = head;
		return ;
	}
	while (head)
	{
		if (head->next == 0 || (*cmp)(head->next->data, data) >= 0)
		{
			new = ft_create_elem(data);
			new->next = head->next;
			head->next = new;
			return ;
		}
		head = head->next;
	}
}
