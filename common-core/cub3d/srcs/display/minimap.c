/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 18:10:28 by llebugle          #+#    #+#             */
/*   Updated: 2025/03/27 18:10:29 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	render_in_minimap(t_data *data, t_minimap *minimap, int i, int j)
{
	int	distance;

	distance = i * i + j * j;
	if (distance <= minimap->radius * minimap->radius)
	{
		draw_transparent_pixel(data, (t_coord){minimap->center_x + j,
			minimap->center_y + i}, 0x808080, 0.5);
		if (distance >= (minimap->radius - 1) * (minimap->radius - 1))
		{
			draw_transparent_pixel(data, (t_coord){minimap->center_x + j,
				minimap->center_y + i}, 0xFFFFFF, 0.8);
		}
	}
}

static int	set_color(t_data *data, t_coord world, int y, int x)
{
	if (x * x + y * y > data->minimap.radius * data->minimap.radius)
		return (-1);
	if (world.x < 0 || world.y < 0 || world.x >= data->map->width
		|| world.y >= data->map->height)
		return (-1);
	if (data->map->matrix[(int)world.y][(int)world.x] == 1)
		return (0xFFFFFF);
	else if (data->map->matrix[(int)world.y][(int)world.x] == 2
		|| data->map->matrix[(int)world.y][(int)world.x] == -2)
		return (0x00FFFF);
	else
		return (-1);
}

static void	render_map_elements(t_data *data, t_minimap *minimap)
{
	int		color;
	t_coord	world;
	int		y;
	int		x;

	y = -minimap->radius;
	while (y <= minimap->radius)
	{
		x = -minimap->radius - 1;
		while (++x <= minimap->radius)
		{
			world.x = data->player.pos_x + (double)x / minimap->ratio;
			world.y = data->player.pos_y + (double)y / minimap->ratio;
			color = set_color(data, world, y, x);
			if (color == -1)
				continue ;
			draw_pixel(&data->s_img, minimap->center_x + x, minimap->center_y
				+ y, color);
		}
		y++;
	}
}

void	render_minimap(t_data *data)
{
	t_minimap	*minimap;
	int			i;
	int			j;

	minimap = &data->minimap;
	i = -(minimap->radius);
	while (i <= minimap->radius)
	{
		j = -(minimap->radius);
		while (j <= minimap->radius)
		{
			render_in_minimap(data, minimap, i, j);
			j++;
		}
		i++;
	}
	render_map_elements(data, &data->minimap);
	render_player_in_minimap(data, &data->minimap);
}
