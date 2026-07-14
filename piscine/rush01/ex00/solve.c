/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solve.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 14:00:05 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/14 19:46:27 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rush01.h"

int	ft_solve(int board[4][4], int view[4][4], int row, int col)
{
	int	i;

	i = 1;
	if (row > 3 || col > 3)
		return (0);
	while (i < 5)
	{
		board[row][col] = i;
		if (ft_is_valid(board, view))
		{
			if (row == 3 && col == 3)
				return (1);
			if (col == 3)
			{
				if (ft_solve(board, view, row + 1, 0))
					return (1);
			}
			else
				if (ft_solve(board, view, row, col + 1))
					return (1);
		}
		i++;
	}
	board[row][col] = 0;
	return (0);
}
