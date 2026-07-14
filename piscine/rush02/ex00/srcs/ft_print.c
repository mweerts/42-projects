/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 16:56:20 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/22 16:16:25 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rush02.h"

t_list	*ft_convert_input(char *input)
{
	int		i;
	int		j;
	int		nb;
	t_list	*lst;

	lst = 0;
	i = ft_strlen(input) - 1;
	while (i >= 0)
	{
		j = 0;
		nb = 0;
		while (i >= 0 && j < 3)
		{
			nb += (input[i] - '0') * ft_power(10, j);
			i--;
			j++;
		}
		ft_list_push_front(&lst, nb, 0);
	}
	return (lst);
}

void	ft_append_number(int nb, t_list *numbers, t_list **resultat)
{
	if (nb >= 100)
	{
		ft_list_push_back(resultat, 0, ft_list_find_str(numbers, nb / 100));
		ft_list_push_back(resultat, 0, ft_list_find_str(numbers, 100));
	}
	nb = nb % 100;
	if (nb == 0)
		return ;
	if (nb <= 20)
		ft_list_push_back(resultat, 0, ft_list_find_str(numbers, nb));
	else
	{
		if ((nb / 10) * 10 != 0)
			ft_list_push_back(resultat, 0,
				ft_list_find_str(numbers, (nb / 10) * 10));
		if (nb % 10 != 0)
			ft_list_push_back(resultat, 0, ft_list_find_str(numbers, nb % 10));
	}
}

void	ft_print_result_list(t_list *resultat)
{
	while (resultat)
	{
		ft_putstr(resultat->str);
		if (resultat->next)
			write(1, " ", 1);
		else
			write(1, "\n", 1);
		resultat = resultat->next;
	}
}

int	ft_check_result(t_list *resultat)
{
	t_list	*tmp;

	tmp = resultat;
	while (tmp)
	{
		if (!(tmp->str))
			return (0);
		tmp = tmp->next;
	}
	return (1);
}

void	ft_print(char	*input, t_list *dict, t_list *multipliers)
{
	t_list	*input_lst;
	t_list	*resultat;
	t_list	*head_input;
	int		multi;

	input_lst = ft_convert_input(input);
	head_input = input_lst;
	multi = ft_list_size(input_lst) - 1;
	resultat = 0;
	if (ft_list_size(input_lst) == 1 && input_lst->number == 0)
		ft_list_push_back(&resultat, 0, ft_list_find_str(dict, 0));
	while (multi >= 0 && input_lst)
	{
		ft_append_number(input_lst->number, dict, &resultat);
		if (input_lst->number != 0 && multi > 0)
			ft_list_push_back(&resultat, 0,
				ft_list_find_str(multipliers, multi));
		input_lst = input_lst->next;
		multi--;
	}
	if (!ft_check_result(resultat))
		ft_putstr("Dict Error\n");
	else
		ft_print_result_list(resultat);
	ft_free_result_and_input(resultat, head_input);
}
