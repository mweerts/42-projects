/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_push_back.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 11:49:36 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/17 13:15:54 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_list.h"

t_list	*ft_list_last(t_list *begin_list)
{
	while (begin_list->next)
		begin_list = begin_list->next;
	return (begin_list);
}

void	ft_list_push_back(t_list **begin_list, void *data)
{
	t_list	*elem;

	if (!(*begin_list))
	{
		*begin_list = ft_create_elem(data);
		return ;
	}
	elem = ft_create_elem(data);
	if (!elem)
		return ;
	ft_list_last(*begin_list)->next = elem;
}
