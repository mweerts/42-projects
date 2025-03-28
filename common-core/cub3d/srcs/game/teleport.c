/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   teleport.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 23:18:59 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/28 20:15:40 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static int	is_tp_possible(t_map *map, int x, int y)
{
	int	**cpy;
	int	i;
	int	j;

	cpy = (int **)ft_calloc((map->height + 1), sizeof(int *));
	if (!cpy)
		return (print_err(MSG_ERR_MALLOC), -1);
	i = -1;
	while (++i < map->height)
	{
		cpy[i] = (int *)ft_calloc(map->width, sizeof(int));
		if (!cpy[i])
			return (free_matrix(cpy, i), print_err(MSG_ERR_MALLOC), -1);
		j = -1;
		while (++j < map->width)
			cpy[i][j] = map->matrix[i][j];
	}
	if (flood_fill(map, cpy, y, x) == ERROR)
		return (0);
	free_matrix(cpy, map->height);
	return (1);
}

static int	teleport_x_axis(t_data *data, double old_x, int dir)
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
			if (data->map->matrix[y][x + count + dir] == 0
				&& is_tp_possible(data->map, x + count + dir, y))
				return (data->player.pos_x += (double)(count + dir), 1);
		count += dir;
	}
	if (x + dir > 1 && x + dir < data->map->width - 1
		&& data->map->matrix[y][x + dir] == 0)
		data->player.pos_x += (double)dir;
	else
		data->player.pos_x = old_x;
	return (1);
}

static int	teleport_y_axis(t_data *data, double old_y, int dir)
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
			if (data->map->matrix[y + count + dir][x] == 0
				&& is_tp_possible(data->map, x, y + count + dir))
				return (data->player.pos_y += (double)(count + dir), 1);
		count += dir;
	}
	if (y + dir > 1 && y + dir < data->map->height - 1
		&& data->map->matrix[y + dir][x] == 0)
		data->player.pos_y += (double)dir;
	else
		data->player.pos_y = old_y;
	return (1);
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
