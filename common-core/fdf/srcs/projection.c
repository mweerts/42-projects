/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 14:36:15 by maxweert          #+#    #+#             */
/*   Updated: 2025/01/18 03:39:07 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	proj_rotate(t_data *data, t_coord *coord)
{
	int	tmp_x;
	int	tmp_y;

	tmp_y = coord->y;
	coord->y = tmp_y * cos(data->cam.x_rot) + coord->z * sin(data->cam.x_rot);
	coord->z = -tmp_y * sin(data->cam.x_rot) + coord->z * cos(data->cam.x_rot);
	tmp_x = coord->x;
	coord->x = tmp_x * cos(data->cam.y_rot) + coord->z * sin(data->cam.y_rot);
	coord->z = -tmp_x * sin(data->cam.y_rot) + coord->z * cos(data->cam.y_rot);
	tmp_x = coord->x;
	tmp_y = coord->y;
	coord->x = tmp_x * cos(data->cam.z_rot) - tmp_y * sin(data->cam.z_rot);
	coord->y = tmp_x * sin(data->cam.z_rot) + tmp_y * cos(data->cam.z_rot);
}

static void	isometric(t_coord *coord)
{
	int	tmp_x;
	int	tmp_y;

	tmp_x = coord->x;
	tmp_y = coord->y;
	coord->x = (tmp_x - tmp_y) * cos(0.523599);
	coord->y = -(coord->z) + (tmp_x + tmp_y) * sin(0.523599);
}

t_coord	project(t_data *data, int x, int y)
{
	t_coord	coord;

	coord.color = data->map.colors_arr[y][x];
	coord.x = x * data->cam.zoom;
	coord.y = y * data->cam.zoom;
	coord.z = data->map.z_arr[y][x] * (data->cam.zoom / data->cam.z_divisor);
	coord.x -= (data->map.width * data->cam.zoom) / 2;
	coord.y -= (data->map.height * data->cam.zoom) / 2;
	proj_rotate(data, &coord);
	if (data->cam.projection == ISOMETRIC)
		isometric(&coord);
	coord.x += (WIDTH - MENU_WIDTH) / 2 + data->cam.x_offset + MENU_WIDTH;
	coord.y += HEIGHT / 2 + data->cam.y_offset;
	return (coord);
}
