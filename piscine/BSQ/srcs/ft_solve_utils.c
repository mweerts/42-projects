/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_solve_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 17:57:57 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/25 18:12:01 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_bsq.h"

void	ft_update_coords(int **matrix, int i, int j, t_square_coord *coords)
{
	coords->max = matrix[i][j];
	coords->max_x = j;
	coords->max_y = i;
}

void	ft_init_coords(t_square_coord *coords)
{
	coords->max = 0;
	coords->max_x = 0;
	coords->max_y = 0;
}

void	ft_init_data(t_read_data *data)
{
	data->col = 0;
	data->row = 0;
	data->line = NULL;
	data->line_len = 0;
}
