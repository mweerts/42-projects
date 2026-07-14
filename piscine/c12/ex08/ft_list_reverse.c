/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_reverse.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 15:36:12 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/15 21:06:52 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_list.h"

void	ft_list_reverse(t_list **begin_list)
{
	t_list	*prev;
	t_list	*head;
	t_list	*next;

	head = *begin_list;
	prev = 0;
	next = 0;
	while (head)
	{
		next = head->next;
		head->next = prev;
		prev = head;
		head = next;
	}
	*begin_list = prev;
}
