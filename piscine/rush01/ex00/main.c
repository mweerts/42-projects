/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 12:05:49 by llebugle          #+#    #+#             */
/*   Updated: 2024/09/14 18:21:32 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rush01.h"

int	main(int ac, char **av)
{
	int	view[4][4];
	int	board[4][4];

	if (ac == 2)
	{
		if (!parse_arguments(av[1], view))
		{
			ft_putstr("Error\n");
			return (0);
		}
		ft_init_board(board);
		if (ft_solve(board, view, 0, 0))
			ft_print_board(board);
		else
			ft_putstr("Error\n");
	}
	else
		ft_putstr("Error\n");
	return (0);
}
