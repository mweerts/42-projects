/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_map.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 01:17:15 by llebugle          #+#    #+#             */
/*   Updated: 2025/04/01 01:17:16 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

/* Renders the map content inside the frame texture without scaling */
void    render_map_content(t_data *data)
{
    int         x;
    int         y;
    int         map_x;
    int         map_y;
    int         cell_size;
    int         color;
    t_coord     cell_pos;
    float       overlay_alpha;
    int         frame_width;
    int         frame_height;
    int         map_area_width;
    int         map_area_height;
    int         map_area_x;
    int         map_area_y;
    int         frame_margin;
    
    // Set a fixed cell size (similar to minimap)
    cell_size = 14;  // Fixed cell size in pixels
    
    // Get the dimensions of the frame
    frame_width = data->tex[TEX_EMPTY_FRAME]->width;
    frame_height = data->tex[TEX_EMPTY_FRAME]->height;
    
    // Define margin from frame edge to map content
    frame_margin = 50;  // Pixels from frame edge to map content
    
    // Calculate the map area dimensions inside the frame
    map_area_width = frame_width - (frame_margin * 2);
    map_area_height = frame_height - (frame_margin * 2);
    
    // Calculate how many cells can fit in the map area
    int cells_x = map_area_width / cell_size;
    int cells_y = map_area_height / cell_size;
    
    // Calculate center position of the frame on screen
    int frame_center_x = WIDTH / 2;
    int frame_center_y = HEIGHT / 2;
    
    // Calculate the top-left corner of the map area relative to the frame
    map_area_x = frame_center_x - (map_area_width / 2);
    map_area_y = frame_center_y - (map_area_height / 2) - 10; // Slight vertical adjustment
    
    // Calculate which part of the map should be visible
    // Center on player position
    int center_map_x = (int)data->player.pos_x;
    int center_map_y = (int)data->player.pos_y;
    
    // Calculate the starting map coordinates (top-left corner of visible area)
    int start_map_x = center_map_x - (cells_x / 2);
    int start_map_y = center_map_y - (cells_y / 2);
    
    // Set transparency level for the cells
    overlay_alpha = 0.9f;
    
    // Draw visible map cells
    for (y = 0; y < cells_y; y++)
    {
        map_y = start_map_y + y;
        
        // Skip if outside map boundaries
        if (map_y < 0 || map_y >= data->map->height)
            continue;
            
        for (x = 0; x < cells_x; x++)
        {
            map_x = start_map_x + x;
            
            // Skip if outside map boundaries
            if (map_x < 0 || map_x >= data->map->width)
                continue;
                
            // Calculate cell position on screen
            cell_pos.x = map_area_x + (x * cell_size);
            cell_pos.y = map_area_y + (y * cell_size);
            
            // Determine cell color based on map content
            if (data->map->matrix[map_y][map_x] == 1)
                color = 0xFFFFFF; // Wall
            else if (data->map->matrix[map_y][map_x] == 2 || 
                    data->map->matrix[map_y][map_x] == -2)
                color = 0x00FFFF; // Special object (interruptor)
            else if (data->map->matrix[map_y][map_x] == 0)
                continue; // Skip empty space (make it transparent)
            else
                color = 0x808080; // Other
            
            // Draw the cell
            for (int cy = 0; cy < cell_size; cy++)
            {
                for (int cx = 0; cx < cell_size; cx++)
                {
                    if (cell_pos.x + cx >= 0 && cell_pos.x + cx < WIDTH &&
                        cell_pos.y + cy >= 0 && cell_pos.y + cy < HEIGHT)
                    {
                        // Draw with transparency
                        draw_transparent_pixel(data, 
                            (t_coord){cell_pos.x + cx, cell_pos.y + cy}, 
                            color, overlay_alpha);
                    }
                }
            }
        }
    }
    
    // Draw player position (always at the center)
    t_coord player_pos;
    player_pos.x = map_area_x + (cells_x / 2) * cell_size;
    player_pos.y = map_area_y + (cells_y / 2) * cell_size;
    
    // Draw player marker
    int player_marker_size = cell_size / 2;
    if (player_marker_size < 2)
        player_marker_size = 2;
    
    for (y = -player_marker_size; y <= player_marker_size; y++)
        for (x = -player_marker_size; x <= player_marker_size; x++)
            if (x*x + y*y <= player_marker_size*player_marker_size)
                if (player_pos.x + x >= 0 && player_pos.x + x < WIDTH &&
                    player_pos.y + y >= 0 && player_pos.y + y < HEIGHT)
                    	draw_pixel(&data->s_img, player_pos.x + x, player_pos.y + y, 0x8b45);
    
    // Draw player direction line
    t_coord dir_end;
    dir_end.x = player_pos.x + (int)(data->player.dir_x * cell_size * 2);
    dir_end.y = player_pos.y + (int)(data->player.dir_y * cell_size * 2);
    draw_line(data, player_pos, dir_end, 0x8b45);
    
    // Draw portals if they are visible in the current view
    if (data->portal1.x != -1)
    {
        // Check if portal1 is visible in the current map view
        if (data->portal1.x >= start_map_x && data->portal1.x < start_map_x + cells_x &&
            data->portal1.y >= start_map_y && data->portal1.y < start_map_y + cells_y)
        {
            t_coord portal1_pos;
            portal1_pos.x = map_area_x + (data->portal1.x - start_map_x) * cell_size + (cell_size / 2);
            portal1_pos.y = map_area_y + (data->portal1.y - start_map_y) * cell_size + (cell_size / 2);
            
            int portal_size = cell_size / 2;
            if (portal_size < 1)
                portal_size = 1;
                
            for (y = -portal_size; y <= portal_size; y++)
            {
                for (x = -portal_size; x <= portal_size; x++)
                {
                    if (x*x + y*y <= portal_size*portal_size)
                    {
                        if (portal1_pos.x + x >= 0 && portal1_pos.x + x < WIDTH &&
                            portal1_pos.y + y >= 0 && portal1_pos.y + y < HEIGHT)
                        {
                            draw_pixel(&data->s_img, portal1_pos.x + x, portal1_pos.y + y, 0x0000FF);
                        }
                    }
                }
            }
        }
    }
    
    if (data->portal2.x != -1)
    {
        // Check if portal2 is visible in the current map view
        if (data->portal2.x >= start_map_x && data->portal2.x < start_map_x + cells_x &&
            data->portal2.y >= start_map_y && data->portal2.y < start_map_y + cells_y)
        {
            t_coord portal2_pos;
            portal2_pos.x = map_area_x + (data->portal2.x - start_map_x) * cell_size + (cell_size / 2);
            portal2_pos.y = map_area_y + (data->portal2.y - start_map_y) * cell_size + (cell_size / 2);
            
            int portal_size = cell_size / 2;
            if (portal_size < 1)
                portal_size = 1;
                
            for (y = -portal_size; y <= portal_size; y++)
            {
                for (x = -portal_size; x <= portal_size; x++)
                {
                    if (x*x + y*y <= portal_size*portal_size)
                    {
                        if (portal2_pos.x + x >= 0 && portal2_pos.x + x < WIDTH &&
                            portal2_pos.y + y >= 0 && portal2_pos.y + y < HEIGHT)
                        {
                            draw_pixel(&data->s_img, portal2_pos.x + x, portal2_pos.y + y, 0xFF8000);
                        }
                    }
                }
            }
        }
    }
}

void show_map(t_data *data)
{
	draw_texture_centered(data, data->tex[TEX_EMPTY_FRAME], (t_coord){WIDTH
		/ 2, HEIGHT / 2}, 0.6);
	render_map_content(data);
	draw_texture_centered(data, data->tex[TEX_FRAME_DETAILS],
		(t_coord){WIDTH / 2 - data->tex[TEX_EMPTY_FRAME]->width / 2
		+ data->tex[TEX_FRAME_DETAILS]->width / 2,
		HEIGHT / 2 + data->tex[TEX_EMPTY_FRAME]->height / 2 + 32}, 0);
}