/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_stack.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 18:52:53 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/19 18:53:11 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_tail.h"

void	ft_append_stack(t_list **head, char *append, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		if (!head)
			*head = ft_create_elem(append[i]);
		else
			ft_list_push_back(head, append[i]);
		i++;
	}
}

void	ft_pop_stack(t_list **head, int maxsize)
{
	int		current_size;
	t_list	*tmp;

	current_size = ft_list_size(*head);
	while (current_size > maxsize)
	{
		tmp = *head;
		*head = (*head)->next;
		free(tmp);
		current_size--;
	}
}
