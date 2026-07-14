/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_reverse_fun.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 20:28:02 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/16 20:50:40 by maxweert         ###   ########.fr       */
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

t_list	*ft_list_at(t_list *begin_list, unsigned int nbr)
{
	if (!begin_list)
		return (0);
	while (begin_list)
	{
		if (nbr == 0)
			return (begin_list);
		begin_list = begin_list->next;
		nbr--;
	}
	return (0);
}

void	ft_swap_list_data(t_list *lst1, t_list *lst2)
{
	void	*tmp;

	tmp = lst1->data;
	lst1->data = lst2->data;
	lst2->data = tmp;
}

void	ft_list_reverse_fun(t_list *begin_list)
{
	int	list_size;
	int	i;

	list_size = ft_list_size(begin_list);
	i = 0;
	while (i < list_size / 2)
	{
		ft_swap_list_data(ft_list_at(begin_list, i),
			ft_list_at(begin_list, list_size - i - 1));
		i++;
	}
}
