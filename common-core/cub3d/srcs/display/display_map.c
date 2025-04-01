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

void    render_map_content(t_data *data)
{
    int         x;
    int         y;
    int         map_x;
    int         map_y;
    int         cell_size;
    int         map_width;
    int         map_height;
    int         start_x;
    int         start_y;
    int         color;
    t_coord     cell_pos;
    float       overlay_alpha;
    int         frame_width;
    int         frame_height;
    int         map_area_width;
    int         map_area_height;
    int         map_area_x;
    int         map_area_y;
    
    // Get the dimensions of the map
    map_width = data->map->width;
    map_height = data->map->height;
    
    // Get the dimensions of the frame
    frame_width = data->tex[TEX_EMPTY_FRAME]->width;
    frame_height = data->tex[TEX_EMPTY_FRAME]->height;
    
    // Define the area within the frame where the map will be drawn
    // Adjust these values to fit your frame's inner area
    map_area_width = (int)(frame_width * 0.8);  // 80% of frame width
    map_area_height = (int)(frame_height * 0.7); // 70% of frame height
    
    // Calculate center position of the frame on screen
    int frame_center_x = WIDTH / 2;
    int frame_center_y = HEIGHT / 2;
    
    // Calculate the top-left corner of the map area relative to the frame
    map_area_x = frame_center_x - (map_area_width / 2);
    map_area_y = frame_center_y - (map_area_height / 2) - 20; // Adjust to position the map within the frame
    
    // Set a maximum and minimum cell size
    int max_cell_size = 15;  // Maximum cell size in pixels
    int min_cell_size = 6;   // Minimum cell size in pixels
    
    // Calculate the cell size to fit the map in the frame
    cell_size = (int)fmin((float)map_area_width / map_width, (float)map_area_height / map_height);
    
    // Limit the cell size
    if (cell_size > max_cell_size)
        cell_size = max_cell_size;
    if (cell_size < min_cell_size)
        cell_size = min_cell_size;
    
    // Calculate map dimensions in pixels
    int map_pixel_width = cell_size * map_width;
    int map_pixel_height = cell_size * map_height;
    
    // Calculate starting position to center the map in the map area
    start_x = map_area_x + ((map_area_width - map_pixel_width) / 2);
    start_y = map_area_y + ((map_area_height - map_pixel_height) / 2);
    
    // Set transparency level for the cells
    overlay_alpha = 0.8f;
    
    // Draw map cells
    for (map_y = 0; map_y < map_height; map_y++)
    {
        for (map_x = 0; map_x < map_width; map_x++)
        {
            // Calculate cell position
            cell_pos.x = start_x + (map_x * cell_size);
            cell_pos.y = start_y + (map_y * cell_size);
            
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
            for (y = 0; y < cell_size; y++)
            {
                for (x = 0; x < cell_size; x++)
                {
                    if (cell_pos.x + x >= 0 && cell_pos.x + x < WIDTH &&
                        cell_pos.y + y >= 0 && cell_pos.y + y < HEIGHT)
                    {
                        // Draw with transparency
                        draw_transparent_pixel(data, 
                            (t_coord){cell_pos.x + x, cell_pos.y + y}, 
                            color, overlay_alpha);
                    }
                }
            }
        }
    }
    
    // Draw player position
    t_coord player_pos;
    player_pos.x = start_x + (int)(data->player.pos_x * cell_size);
    player_pos.y = start_y + (int)(data->player.pos_y * cell_size);
    
    // Draw player marker
    int player_marker_size = cell_size / 2;
    if (player_marker_size < 2)
        player_marker_size = 2;
    
    for (y = -player_marker_size; y <= player_marker_size; y++)
    {
        for (x = -player_marker_size; x <= player_marker_size; x++)
        {
            if (x*x + y*y <= player_marker_size*player_marker_size)
            {
                if (player_pos.x + x >= 0 && player_pos.x + x < WIDTH &&
                    player_pos.y + y >= 0 && player_pos.y + y < HEIGHT)
                {
                    draw_pixel(&data->s_img, player_pos.x + x, player_pos.y + y, 0xFF0000);
                }
            }
        }
    }
    
    // Draw player direction line
    t_coord dir_end;
    dir_end.x = player_pos.x + (int)(data->player.dir_x * cell_size * 2);
    dir_end.y = player_pos.y + (int)(data->player.dir_y * cell_size * 2);
    draw_line(data, player_pos, dir_end, 0xFF0000);
    
    // Draw portals if they exist
    if (data->portal1.x != -1)
    {
        t_coord portal1_pos;
        portal1_pos.x = start_x + (data->portal1.x * cell_size) + (cell_size / 2);
        portal1_pos.y = start_y + (data->portal1.y * cell_size) + (cell_size / 2);
        
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
    
    if (data->portal2.x != -1)
    {
        t_coord portal2_pos;
        portal2_pos.x = start_x + (data->portal2.x * cell_size) + (cell_size / 2);
        portal2_pos.y = start_y + (data->portal2.y * cell_size) + (cell_size / 2);
        
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