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

int	render_player_in_minimap(t_data *data, t_minimap *minimap);

void	init_minimap(t_minimap *minimap)
{
	ft_memset(minimap, 0, sizeof(t_minimap));
	if (WIDTH / 12 > 0)
		minimap->radius = WIDTH / 16;
	minimap->center_x = WIDTH - minimap->radius - MINIMAP_OFFSET;
	minimap->center_y = HEIGHT - minimap->radius - MINIMAP_OFFSET;
}

void render_in_minimap(t_data *data, t_minimap *minimap, int i, int j)
{
    int distance;
    
    distance = i * i + j * j;
    if (distance <= minimap->radius * minimap->radius)
    {
        draw_transparent_pixel(data, (t_coord){minimap->center_x + j, minimap->center_y + i}, 0x808080, 0.5);
        if (distance >= (minimap->radius - 1) * (minimap->radius - 1))
        {
            draw_transparent_pixel(data, (t_coord){minimap->center_x + j, minimap->center_y + i}, CROSS_COLOR, 0.8);
        }
    }
}

void render_map_elements(t_data *data, t_minimap *minimap)
{
    int map_x, map_y;
    int screen_x, screen_y;
    float scale_factor;
    int max_view_distance;
    int cell_size;
    int color;
    int distance_squared;
    
    // Calculate scale factor based on minimap radius
    // This determines how much map area to show relative to minimap size
    scale_factor = (float)minimap->radius / 12.0f;
    
    // Maximum distance from player to show map elements (in map units)
    max_view_distance = minimap->radius / scale_factor;
    
    // Size of each map cell in the minimap
    cell_size = minimap->radius / max_view_distance;
    if (cell_size < 1) cell_size = 1;
    
    // Iterate through visible map area around player
    for (int y = -max_view_distance; y <= max_view_distance; y++)
    {
        for (int x = -max_view_distance; x <= max_view_distance; x++)
        {
            // Check if within circle boundary in map coordinates
            if (x*x + y*y > max_view_distance*max_view_distance)
                continue;
                
            // Calculate map coordinates
            map_x = (int)data->player.pos_x + x;
            map_y = (int)data->player.pos_y + y;
            
            // Skip if outside map boundaries
            if (map_x < 0 || map_y < 0 || map_x >= data->map->width || map_y >= data->map->height)
                continue;
            
            // Determine color based on map content
            if (data->map->matrix[map_y][map_x] == 1)  // Wall
                color = 0xFFFFFFFF;  // White
            else if (data->map->matrix[map_y][map_x] == 2)  // Portal
                color = 0xFF00FFFF;  // Cyan
            else  // Empty space
                continue;  // Skip drawing empty spaces
            
            // Calculate pixel coordinates in minimap
            screen_x = minimap->center_x + (int)(x * scale_factor);
            screen_y = minimap->center_y + (int)(y * scale_factor);
            
            // Calculate actual cell size - keep walls at original size
            // Note: No longer reducing the size by 0.9 to maintain wall thickness
            int actual_cell_size = cell_size;
            if (actual_cell_size < 1) actual_cell_size = 1;
            
            // Draw cell
            for (int cy = -actual_cell_size/2; cy <= actual_cell_size/2; cy++)
            {
                for (int cx = -actual_cell_size/2; cx <= actual_cell_size/2; cx++)
                {
                    int px = screen_x + cx;
                    int py = screen_y + cy;
                    
                    // Make sure pixel is within the minimap circle
                    distance_squared = (px - minimap->center_x) * (px - minimap->center_x) + 
                                      (py - minimap->center_y) * (py - minimap->center_y);
                    
                    if (distance_squared <= minimap->radius * minimap->radius)
                    {
                        draw_pixel(&data->s_img, px, py, color);
                    }
                }
            }
        }
    }
}

void	set_minimap(t_data *data)
{
	t_minimap	*minimap;
	int i;
	int j;
	

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

