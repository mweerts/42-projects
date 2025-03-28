/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 18:12:39 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/28 16:09:39 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	draw_fps(t_data *data)
{
	char	*tmp;

	tmp = ft_itoa(data->s_fps.fps);
	if (tmp)
	{
		mlx_string_put(data->s_mlx.mlx, data->s_mlx.win, 50, 50, TEXT_COLOR,
			tmp);
		free(tmp);
		tmp = NULL;
	}
}

void	set_cross(t_data *data)
{
	int	i;
	int	x;
	int	y;
	int	size;

	size = CROSS_SIZE;
	if (size % 2 == 0)
		size += 1;
	x = WIDTH / 2 - size / 2;
	y = HEIGHT / 2;
	i = 0;
	while (i < size)
	{
		draw_pixel(&data->s_img, x + i, y, CROSS_COLOR);
		i++;
	}
	x = WIDTH / 2;
	y = HEIGHT / 2 - size / 2;
	i = 0;
	while (i < size)
	{
		draw_pixel(&data->s_img, x, y + i, CROSS_COLOR);
		i++;
	}
}

static void	init_minimap(t_data *data)
{
	t_minimap *minimap;

	minimap = &data->minimap;
	minimap->angle = atan2(data->player.dir_x, -data->player.dir_y);
	minimap->tile_size = MINMAP_SIZE / MINMAP_RANGE;
}

static void	compute_minimap(t_data *data, int x, int y)
{
	t_minimap	*minimap;
	double		dx;
	double		dy;

	minimap = &data->minimap;
	dx = (x / (double)(minimap->tile_size) - (((double)MINMAP_SIZE / 2) / (double)minimap->tile_size));
	dy = (y / (double)minimap->tile_size - (((double)MINMAP_SIZE / 2) / (double)minimap->tile_size));
	minimap->tmp_x = data->player.pos_x + dx * cos(data->minimap.angle) - dy * sin(data->minimap.angle);
	minimap->tmp_y = data->player.pos_y + dx * sin(data->minimap.angle) + dy * cos(data->minimap.angle);
}

void	draw_minimap(t_data *data)
{
	int	x;
	int	y;

	y = -1;
	init_minimap(data);
	while(++y < MINMAP_SIZE)
	{
		x = -1;
		while(++x < MINMAP_SIZE)
		{
			compute_minimap(data, x, y);
			if (data->minimap.tmp_x >= 0 && data->minimap.tmp_y >= 0 && data->minimap.tmp_x < data->map->width && data->minimap.tmp_y < data->map->height)
			{
				if (data->map->matrix[(int)data->minimap.tmp_y][(int)data->minimap.tmp_x] == 0)
					draw_pixel(&data->minimap.s_img, x, y, 0xFF00FF00);
				else if ((data->map->matrix[(int)data->minimap.tmp_y][(int)data->minimap.tmp_x] == 2 || data->map->matrix[(int)data->minimap.tmp_y][(int)data->minimap.tmp_x] == -2))
					draw_pixel(&data->minimap.s_img, x, y, 0xFF0000FF);
				else
					draw_pixel(&data->minimap.s_img, x, y, 0xFFFF0000);
			}
			else
				draw_pixel(&data->minimap.s_img, x, y, 0xFFFF0000);
		}
	}
}
