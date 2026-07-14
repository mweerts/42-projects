/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_map_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 17:05:21 by llebugle          #+#    #+#             */
/*   Updated: 2025/04/02 17:05:22 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static inline int	is_in_map_bounds(t_data *data, int x, int y)
{
	return (x >= 0 && x < data->map->width && y >= 0 && y < data->map->height);
}

static void	draw_cell(t_data *data, t_coord cell_pos, int color, double alpha)
{
	int		cx;
	int		cy;
	t_coord	pixel;

	cy = 0;
	while (cy < MAP_CELL_SIZE)
	{
		cx = 0;
		while (cx < MAP_CELL_SIZE)
		{
			pixel.x = cell_pos.x + cx;
			pixel.y = cell_pos.y + cy;
			if (pixel.x >= 0 && pixel.x < WIDTH && pixel.y >= 0
				&& pixel.y < HEIGHT)
				draw_transparent_pixel(data, pixel, color, alpha);
			cx++;
		}
		cy++;
	}
}

static inline int	is_cell_outside_view(t_coord cell_pos, t_fullmap *map)
{
	return (cell_pos.x + map->cell_size < map->map_area.x
		|| cell_pos.x >= map->map_area.x + (map->cells_x * map->cell_size)
		|| cell_pos.y + map->cell_size < map->map_area.y
		|| cell_pos.y >= map->map_area.y + (map->cells_y * map->cell_size));
}

static int	set_color(t_data *data, t_coord pos)
{
	if (data->map->matrix[(int)pos.y][(int)pos.x] == 1)
		return (0xFFFFFF);
	else if (data->map->matrix[(int)pos.y][(int)pos.x] == 2)
		return (MAIN_YELLOW);
	else if (data->map->matrix[(int)pos.y][(int)pos.x] == 3)
		return (MAIN_GREEN);
	else if (data->map->matrix[(int)pos.y][(int)pos.x] == 0)
		return (-1);
	else
		return (0x808080);
}

void	draw_map_cells(t_data *data, t_fullmap *map)
{
	int		x;
	int		y;
	t_coord	pos;
	t_coord	cell_pos;
	int		color;

	y = -2;
	while (++y < map->cells_y + 1)
	{
		pos.y = (int)map->start.y + y;
		x = -2;
		while (++x < map->cells_x + 1)
		{
			pos.x = map->start.x + x;
			cell_pos.x = map->map_area.x + (x * map->cell_size) - map->offset_x;
			cell_pos.y = map->map_area.y + (y * map->cell_size) - map->offset_y;
			if (!is_in_map_bounds(data, pos.x, pos.y)
				|| is_cell_outside_view(cell_pos, map))
				continue ;
			color = set_color(data, pos);
			if (color != -1)
				draw_cell(data, cell_pos, color, 0.8);
		}
	}
}
