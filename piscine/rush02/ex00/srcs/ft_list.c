/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 15:56:52 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/22 17:32:01 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rush02.h"

t_list	*ft_create_elem(int nb, char *str)
{
	t_list	*elem;

	elem = (t_list *)malloc(sizeof(t_list));
	if (!elem)
		return (0);
	elem->number = nb;
	elem->str = str;
	elem->next = 0;
	return (elem);
}

t_list	*ft_list_last(t_list *begin_list)
{
	while (begin_list->next)
		begin_list = begin_list->next;
	return (begin_list);
}

void	ft_list_push_back(t_list **begin_list, int nb, char *str)
{
	t_list	*elem;

	if (!(*begin_list))
	{
		*begin_list = ft_create_elem(nb, str);
		return ;
	}
	elem = ft_create_elem(nb, str);
	if (!elem)
		return ;
	ft_list_last(*begin_list)->next = elem;
}

void	ft_list_push_front(t_list **begin_list, int nb, char *str)
{
	t_list	*elem;

	elem = ft_create_elem(nb, str);
	if (!elem)
		return ;
	elem->next = *begin_list;
	*begin_list = elem;
}

int	ft_list_key_exist(t_list *begin_list, int key)
{
	while (begin_list)
	{
		if (begin_list->number == key)
			return (1);
		begin_list = begin_list->next;
	}
	return (0);
}
