/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 19:19:18 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/22 16:36:09 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rush02.h"

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

char	*ft_list_find_str(t_list *begin_list, int number)
{
	while (begin_list)
	{
		if (begin_list->number == number)
			return (begin_list->str);
		begin_list = begin_list->next;
	}
	return (0);
}

int	ft_list_find_number(t_list *begin_list, char *str)
{
	while (begin_list)
	{
		if (ft_strcmp(begin_list->str, str) == 0)
			return (begin_list->number);
		begin_list = begin_list->next;
	}
	return (0);
}
