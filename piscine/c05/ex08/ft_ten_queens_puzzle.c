/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ten_queens_puzzle.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 12:10:01 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/08 16:43:46 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_put_tab(int tab[10][10])
{
	int		i;
	int		j;
	char	c;

	i = 0;
	while (i < 10)
	{
		j = 0;
		while (j < 10)
		{
			if (tab[j][i])
			{
				c = j + '0';
				write(1, &c, 1);
				break ;
			}
			j++;
		}
		i++;
	}
	write(1, "\n", 1);
}

int	ft_check_position_isvalid(int tab[10][10], int row, int col)
{
	int		i;
	int		j;

	i = 0;
	while (i < col)
		if (tab[row][i++])
			return (0);
	i = row;
	j = col;
	while (i >= 0 && j >= 0)
		if (tab[i--][j--])
			return (0);
	i = row;
	j = col;
	while (j >= 0 && i < 10)
	{
		if (tab[i][j])
			return (0);
		i++;
		j--;
	}
	return (1);
}

int	ft_ten_queens_puzzle_util(int tab[10][10], int col, int *count)
{
	int	i;
	int	res;

	i = 0;
	res = 0;
	if (col == 10)
	{
		ft_put_tab(tab);
		*count += 1;
		return (1);
	}
	while (i < 10)
	{
		if (ft_check_position_isvalid(tab, i, col))
		{
			tab[i][col] = 1;
			res = ft_ten_queens_puzzle_util(tab, col + 1, count);
			tab[i][col] = 0;
		}
		i++;
	}
	return (res);
}

int	ft_ten_queens_puzzle(void)
{
	int	tab[10][10];
	int	i;
	int	j;
	int	count;

	i = 0;
	count = 0;
	while (i < 10)
	{
		j = 0;
		while (j < 10)
		{
			tab[i][j] = 0;
			j++;
		}
		i++;
	}
	ft_ten_queens_puzzle_util(tab, 0, &count);
	return (count);
}
