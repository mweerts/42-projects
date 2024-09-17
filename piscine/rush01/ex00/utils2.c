/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 12:14:10 by llebugle          #+#    #+#             */
/*   Updated: 2024/09/14 20:01:41 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rush01.h"

int	check_argument(char *str)
{
	int	i;

	i = ft_strlen(str);
	if (i != 4 * 4 + ((4 * 4) - 1))
		return (1);
	i--;
	while (i >= 0)
	{
		if (i % 2 == 0)
		{
			if (str[i] < '1' || str[i] > '4')
				return (1);
			i--;
		}
		else
		{
			if (str[i] != ' ')
				return (1);
			i--;
		}
	}
	return (0);
}

int	parse_arguments(char *str, int tab[4][4])
{
	int	index;
	int	i;
	int	j;

	i = 0;
	index = 0;
	if (check_argument(str) == 1)
		return (0);
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			tab[i][j] = str[index] - '0';
			index++;
			if (str[index] != '\0')
				index++;
			j++;
		}
		i++;
	}
	return (1);
}

void	ft_set_tab_zero(int tab[4])
{
	int	i;

	i = 0;
	while (i < 4)
	{
		tab[i] = 0;
		i++;
	}
}

int	ft_is_valid(int board[4][4], int view[4][4])
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (!ft_check_col_up(board, i, view[0][i]))
			return (0);
		if (!ft_check_col_down(board, i, view[1][i]))
			return (0);
		if (!ft_check_row_left(board, i, view[2][i]))
			return (0);
		if (!ft_check_row_right(board, i, view[3][i]))
			return (0);
		i++;
	}
	return (1);
}
