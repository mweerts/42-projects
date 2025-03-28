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

void render_map_elements(t_data *data, t_minimap *minimap)
{
    int map_x, map_y;
    int screen_x, screen_y;
    int cell_size;
    int color;
    int distance_squared;
    
    // Size of each map cell in the minimap
    cell_size = minimap->radius / minimap->max_view_distance;
    if (cell_size < 1) cell_size = 1;
    
    // Iterate through visible map area around player
    for (int y = -minimap->max_view_distance; y <= minimap->max_view_distance; y++)
    {
        for (int x = -minimap->max_view_distance; x <= minimap->max_view_distance; x++)
        {
            // Check if within circle boundary in map coordinates
            if (x*x + y*y > minimap->max_view_distance*minimap->max_view_distance)
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
            screen_x = minimap->center_x + (int)(x * minimap->scale_factor);
            screen_y = minimap->center_y + (int)(y * minimap->scale_factor);
            
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
