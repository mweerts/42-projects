/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   optimize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 17:10:03 by maxweert          #+#    #+#             */
/*   Updated: 2024/11/28 17:56:08 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	ft_next_match(char *curr, char *next)
{
	if (ft_strncmp(curr, "pa\n", 3) == 0 && ft_strncmp(next, "pb\n", 3) == 0)
		return (1);
	if (ft_strncmp(curr, "pb\n", 3) == 0 && ft_strncmp(next, "pa\n", 3) == 0)
		return (1);
	if (ft_strncmp(curr, "rb\n", 3) == 0 && ft_strncmp(next, "rrb\n", 4) == 0)
		return (1);
	if (ft_strncmp(curr, "rrb\n", 4) == 0 && ft_strncmp(next, "rb\n", 3) == 0)
		return (1);
	if (ft_strncmp(curr, "ra\n", 3) == 0 && ft_strncmp(next, "rra\n", 4) == 0)
		return (1);
	if (ft_strncmp(curr, "rra\n", 4) == 0 && ft_strncmp(next, "ra\n", 3) == 0)
		return (1);
	if (ft_strncmp(curr, "rb\n", 3) == 0 && ft_strncmp(next, "rrb\n", 4) == 0)
		return (1);
	if (ft_strncmp(curr, "sb\n", 4) == 0 && ft_strncmp(next, "sb\n", 3) == 0)
		return (1);
	if (ft_strncmp(curr, "sa\n", 4) == 0 && ft_strncmp(next, "sa\n", 3) == 0)
		return (1);
	return (0);
}

void	ft_remove_neutrals(t_list **op_lst)
{
	t_list	*head;
	t_list	*previous;

	head = *op_lst;
	previous = NULL;
	while (head && head->next)
	{
		if (ft_next_match(head->content, head->next->content))
		{
			if (previous)
				previous->next = head->next->next;
			else
				*op_lst = head->next->next;
			ft_lstdelone(head->next, &free);
			ft_lstdelone(head, &free);
			head = *op_lst;
			previous = NULL;
		}
		else
		{
			previous = head;
			head = head->next;
		}
	}
}
