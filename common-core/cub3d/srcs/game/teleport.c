/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   teleport.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 23:18:59 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/28 00:06:53 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static void	teleport_x_axis(t_data *data, double old_x, int dir)
{
	int	x;
	int	y;
	int	count;

	x = (int)data->player.pos_x;
	y = (int)data->player.pos_y;
	count = dir;
	while (x + count > 1 && x + count < data->map->width - 1)
	{
		if (data->map->matrix[y][x + count] == 2 || data->map->matrix[y][x
			+ count] == -2)
		{
			if (data->map->matrix[y][x + count + dir] == 0)
			{
				data->player.pos_x += (double)(count + dir);
				return ;
			}
		}
		count += dir;
	}
	if (x + dir > 1 && x + dir < data->map->width - 1
		&& data->map->matrix[y][x + dir] == 0)
		data->player.pos_x += (double)dir;
	else
		data->player.pos_x = old_x;
}

static void	teleport_y_axis(t_data *data, double old_y, int dir)
{
	int	x;
	int	y;
	int	count;

	x = (int)data->player.pos_x;
	y = (int)data->player.pos_y;
	count = dir;
	while (y + count > 1 && y + count < data->map->height - 1)
	{
		if (data->map->matrix[y + count][x] == 2 || data->map->matrix[y
			+ count][x] == -2)
		{
			if (data->map->matrix[y + count + dir][x] == 0)
			{
				data->player.pos_y += (double)(count + dir);
				return ;
			}
		}
		count += dir;
	}
	if (y + dir > 1 && y + dir < data->map->height - 1
		&& data->map->matrix[y + dir][x] == 0)
		data->player.pos_y += (double)dir;
	else
		data->player.pos_y = old_y;
}

void	teleport(t_data *data, double old_x, double old_y)
{
	if ((int)old_x < (int)data->player.pos_x)
		teleport_x_axis(data, old_x, 1);
	else if ((int)old_x > (int)data->player.pos_x)
		teleport_x_axis(data, old_x, -1);
	else if ((int)old_y < (int)data->player.pos_y)
		teleport_y_axis(data, old_y, 1);
	else if ((int)old_y > (int)data->player.pos_y)
		teleport_y_axis(data, old_y, -1);
}
