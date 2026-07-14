/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rush03.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 18:31:25 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/08 11:16:50 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	ft_putchar(char c);

void	rush(int x, int y)
{
	int	row;
	int	col;

	row = 0;
	if (x <= 0 || y <= 0)
		return ;
	while (row < y)
	{
		col = 0;
		while (col < x)
		{
			if (col == 0 && (row == 0 || row == y - 1))
				ft_putchar('A');
			else if (col == x - 1 && (row == 0 || row == y - 1))
				ft_putchar('C');
			else if (col != 0 && col != x - 1 && row != 0 && row != y - 1)
				ft_putchar(' ');
			else if ((row != 0 && row != y - 1) || (col != 0 && col != x - 1))
				ft_putchar('B');
			col++;
		}
		ft_putchar('\n');
		row++;
	}
}
