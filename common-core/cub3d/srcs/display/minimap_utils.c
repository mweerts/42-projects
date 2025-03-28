/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 20:37:01 by llebugle          #+#    #+#             */
/*   Updated: 2025/03/27 20:37:02 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static int	render_elements(t_data *data, t_minimap *minimap, int x)
{
	return (0);
}

static int should_draw(t_data *data, t_coord world, int x, int y)
{
	// t_coord world;
	
	if (x * x + y * y > data->minimap.radius * data->minimap.radius)
		return (0) ;
	// Calculate exact world coordinates for this minimap pixel
	// This is the key to fluid movement
	// world.x = data->player.pos_x + (double)x / minimap->ratio;
	// world.y = data->player.pos_y + (double)y / minimap->ratio;
	if (world.x < 0 || world.y < 0 || world.x >= data->map->width
		|| world.y >= data->map->height)
		return (0) ;
	return (true);
}

void	render_map_elements(t_data *data, t_minimap *minimap)
{
	int		color;
	double	distance_squared;
	int		y;
	int		x;

	double px, py;
	t_coord world;
	int screen_px, screen_py;
	y = -minimap->radius;
	while (y <= minimap->radius)
	{
		x = -minimap->radius;
		while (x <= minimap->radius)
		{
			// Check if pixel is within circle
			world.x = data->player.pos_x + (double)x / minimap->ratio;
			world.y = data->player.pos_y + (double)y / minimap->ratio;
			if (!should_draw(data, world, x, y))
			{
				x++;
				continue ;
			}
			// // Calculate exact world coordinates for this minimap pixel
			// // This is the key to fluid movement
			// if (world_x < 0 || world_y < 0 || world_x >= data->map->width
			// 	|| world_y >= data->map->height)
			// {
			// 	x++;
			// 	continue ;
			// }
			// Determine color based on map content
			if (data->map->matrix[(int)world.y][(int)world.x] == 1)
				// Wall
				color = 0xFFFFFFFF;
			// White
			else if (data->map->matrix[(int)world.y][(int)world.x] == 2
				|| data->map->matrix[(int)world.y][(int)world.x] == -2)
				// Portal
				color = 0xFF00FFFF;
			// Cyan
			else
			{
				// Empty space
				x++;
				//
				continue ;
			}
			// Calculate screen coordinates
			screen_px = minimap->center_x + x;
			screen_py = minimap->center_y + y;
			// Draw the pixel
			draw_pixel(&data->s_img, screen_px, screen_py, color);
			x++;
		}
		y++;
	}
}

