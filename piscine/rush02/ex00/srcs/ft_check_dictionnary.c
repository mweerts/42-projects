/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_dictionnary.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 15:46:33 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/22 17:08:57 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rush02.h"

int	ft_check_dict(t_list **dictionnary)
{
	t_list	*head;
	int		i;

	head = *dictionnary;
	i = 0;
	while (head)
	{
		if (head->next)
		{
			if (ft_list_key_exist(head->next, head->number))
				return (0);
		}
		head = head->next;
	}
	while (i <= 20)
		if (ft_list_key_exist(*dictionnary, i++) == 0)
			return (0);
	i = 30;
	while (i <= 100)
	{
		if (ft_list_key_exist(*dictionnary, i) == 0)
			return (0);
		i += 10;
	}
	return (1);
}

int	ft_check_multipliers(t_list **multipliers)
{
	t_list	*head;
	int		i;

	head = *multipliers;
	i = 1;
	while (head)
	{
		if (head->next)
		{
			if (ft_list_key_exist(head->next, head->number))
				return (0);
		}
		head = head->next;
	}
	head = *multipliers;
	while (i <= 12)
	{
		if (ft_list_key_exist(head, i) == 0)
			return (0);
		i++;
	}
	return (1);
}
