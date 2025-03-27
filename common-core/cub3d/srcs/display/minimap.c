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

int		render_player_in_minimap(t_data *data, t_minimap *minimap);

void	init_minimap(t_minimap *minimap)
{
	ft_memset(minimap, 0, sizeof(t_minimap));
	if (WIDTH / 12 > 0)
		minimap->radius = WIDTH / 16;
	minimap->center_x = WIDTH - minimap->radius - MINIMAP_OFFSET;
	minimap->center_y = HEIGHT - minimap->radius - MINIMAP_OFFSET;
	// Calculate scale factor based on minimap radius
	// This determines how much map area to show relative to minimap size
	minimap->scale_factor = (float)minimap->radius / 12.0f;
	// Maximum distance from player to show map elements (in map units)
	minimap->max_view_distance = minimap->radius / minimap->scale_factor;
}

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
				minimap->center_y + i}, CROSS_COLOR, 0.8);
		}
	}
}

void render_map_elements(t_data *data, t_minimap *minimap);

void	set_minimap(t_data *data)
{
	t_minimap	*minimap;
	int			i;
	int			j;

	minimap = &data->minimap;
	i = -(minimap->radius);
	// display_orientations(data, &data->minimap);
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

int	render_minimap(t_data *data)
{
	set_minimap(data);
	return (0);
}

