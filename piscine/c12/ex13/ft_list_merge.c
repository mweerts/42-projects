/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_merge.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 21:00:27 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/15 21:09:17 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_list.h"

void	ft_list_merge(t_list **begin_list, t_list *begin_list2)
{
	t_list	*head;

	head = *begin_list;
	if (!head)
	{
		*begin_list = begin_list2;
		return ;
	}
	while (head->next)
	{
		head = head->next;
	}
	head->next = begin_list2;
}
