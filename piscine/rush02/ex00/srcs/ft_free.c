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

int	ft_free_file_content(char *file_content)
{
	free(file_content);
	return (0);
}

void	ft_list_double_free(t_list *begin_list)
{
	t_list	*tmp;

	while (begin_list)
	{
		tmp = begin_list;
		begin_list = begin_list->next;
		free(tmp->str);
		free(tmp);
	}
}

void	ft_list_free(t_list *begin_list)
{
	t_list	*tmp;

	while (begin_list)
	{
		tmp = begin_list;
		begin_list = begin_list->next;
		free(tmp);
	}
}

void	ft_free_result_and_input(t_list *resultat, t_list *input)
{
	ft_list_free(resultat);
	ft_list_double_free(input);
}

int	ft_free_dict_and_mult(t_list *dictionnary, t_list *multipliers)
{
	if (dictionnary)
		ft_list_double_free(dictionnary);
	if (multipliers)
		ft_list_double_free(multipliers);
	return (0);
}
