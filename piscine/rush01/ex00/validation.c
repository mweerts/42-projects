/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 12:57:32 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/14 20:03:06 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rush01.h"

void	ft_set_deg(int *i, int *count, int *max, int exist[4])
{
	*i = -1;
	*count = 0;
	*max = 0;
	ft_set_tab_zero(exist);
}

int	ft_check_row_left(int board[4][4], int row, int left_view)
{
	int	i;
	int	count;
	int	max;
	int	exist[4];

	ft_set_deg(&i, &count, &max, exist);
	while (++i < 4)
	{
		if (board[row][i] == 0)
			return (1);
		if (exist[board[row][i] - 1])
			return (0);
		else
			exist[board[row][i] - 1] = 1;
		if (board[row][i] > max)
		{
			max = board[row][i];
			count++;
		}
		if (count > left_view)
			return (0);
	}
	if (count == left_view)
		return (1);
	return (0);
}

int	ft_check_row_right(int board[4][4], int row, int right_view)
{
	int	i;
	int	count;
	int	max;

	i = 3;
	count = 0;
	max = 0;
	while (i >= 0)
	{
		if (board[row][i] == 0)
			return (1);
		if (board[row][i] > max)
		{
			max = board[row][i];
			count++;
		}
		if (count > right_view)
			return (0);
		i--;
	}
	if (count == right_view)
		return (1);
	return (0);
}

int	ft_check_col_up(int board[4][4], int col, int up_view)
{
	int	i;
	int	count;
	int	max;
	int	exist[4];

	ft_set_deg(&i, &count, &max, exist);
	while (++i < 4)
	{
		if (board[i][col] == 0)
			return (1);
		if (exist[board[i][col] - 1])
			return (0);
		else
			exist[board[i][col] - 1] = 1;
		if (board[i][col] > max)
		{
			max = board[i][col];
			count++;
		}
		if (count > up_view)
			return (0);
	}
	if (count == up_view)
		return (1);
	return (0);
}

int	ft_check_col_down(int board[4][4], int col, int down_view)
{
	int	i;
	int	count;
	int	max;

	i = 3;
	count = 0;
	max = 0;
	while (i >= 0)
	{
		if (board[i][col] == 0)
			return (1);
		if (board[i][col] > max)
		{
			max = board[i][col];
			count++;
		}
		if (count > down_view)
			return (0);
		i--;
	}
	if (count == down_view)
		return (1);
	return (0);
}
