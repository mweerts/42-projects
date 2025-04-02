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

/* Helper function to check if a point is inside map boundaries */
static int	is_in_map_bounds(t_data *data, int map_x, int map_y)
{
	return (map_x >= 0 && map_x < data->map->width && map_y >= 0
		&& map_y < data->map->height);
}

/* Draw a portal circle at the specified location */
static void	draw_portal_circle(t_data *data, t_coord portal_pos, int size,
		int color)
{
	int		x;
	int		y;
	t_coord	pixel;

	for (y = -size; y <= size; y++)
	{
		for (x = -size; x <= size; x++)
		{
			if (x * x + y * y <= size * size)
			{
				pixel.x = portal_pos.x + x;
				pixel.y = portal_pos.y + y;
				if (pixel.x >= 0 && pixel.x < WIDTH && pixel.y >= 0
					&& pixel.y < HEIGHT)
				{
					draw_pixel(&data->s_img, pixel.x, pixel.y, color);
				}
			}
		}
	}
}

/* Initialize map display parameters */
static void	init_show_map(t_data *data, t_show_map *map)
{
	int		frame_width;
	int		frame_height;
	int		map_area_width;
	int		map_area_height;
	t_coord	frame_center;

	map->cell_size = MAP_CELL_SIZE;
	frame_width = data->tex[TEX_EMPTY_FRAME]->width;
	frame_height = data->tex[TEX_EMPTY_FRAME]->height;
	map_area_width = frame_width - (FRAME_MARGIN * 2);
	map_area_height = frame_height - (FRAME_MARGIN * 2 + 20);
	map->cells_x = map_area_width / map->cell_size;
	map->cells_y = map_area_height / map->cell_size;
	frame_center.x = WIDTH / 2;
	frame_center.y = HEIGHT / 2;
	map->map_area.x = frame_center.x - (map_area_width / 2);
	map->map_area.y = frame_center.y - (map_area_height / 2) - 10;
	// Calculate the starting map coordinates
	map->start_map_x = data->player.pos_x - (map->cells_x / 2.0);
	map->start_map_y = data->player.pos_y - (map->cells_y / 2.0);
	// Calculate fractional offsets for smooth movement
	map->offset_x = (map->start_map_x - floor(map->start_map_x))
		* map->cell_size;
	map->offset_y = (map->start_map_y - floor(map->start_map_y))
		* map->cell_size;
	// Adjust start position to whole numbers
	map->start_map_x = floor(map->start_map_x);
	map->start_map_y = floor(map->start_map_y);
	// Set player position at center of map view
	map->player_pos.x = map->map_area.x + (map->cells_x / 2.0) * map->cell_size;
	map->player_pos.y = map->map_area.y + (map->cells_y / 2.0) * map->cell_size;
}

/* Draw a single cell with specified color and transparency */
static void	draw_cell(t_data *data, t_coord cell_pos, int color, double alpha)
{
	int		cx;
	int		cy;
	t_coord	pixel;

	for (cy = 0; cy < MAP_CELL_SIZE; cy++)
	{
		for (cx = 0; cx < MAP_CELL_SIZE; cx++)
		{
			pixel.x = cell_pos.x + cx;
			pixel.y = cell_pos.y + cy;
			if (pixel.x >= 0 && pixel.x < WIDTH && pixel.y >= 0
				&& pixel.y < HEIGHT)
			{
				draw_transparent_pixel(data, pixel, color, alpha);
			}
		}
	}
}

/* Draw map cells based on map content */
static void	draw_map_cells(t_data *data, t_show_map *map)
{
	int		x;
	int		y;
	int		map_x_int;
	int		map_y_int;
	t_coord	cell_pos;
	int		color;

	// We need one extra cell in each direction to handle scrolling
	for (y = -1; y < map->cells_y + 1; y++)
	{
		map_y_int = (int)map->start_map_y + y;
		if (!is_in_map_bounds(data, 0, map_y_int))
			continue ;
		for (x = -1; x < map->cells_x + 1; x++)
		{
			map_x_int = (int)map->start_map_x + x;
			if (!is_in_map_bounds(data, map_x_int, map_y_int))
				continue ;
			// Apply fractional offset for smooth scrolling
			cell_pos.x = map->map_area.x + (x * map->cell_size) - map->offset_x;
			cell_pos.y = map->map_area.y + (y * map->cell_size) - map->offset_y;
			// Skip cells that would be completely outside the visible area
			if (cell_pos.x + map->cell_size < map->map_area.x
				|| cell_pos.x >= map->map_area.x + (map->cells_x
					* map->cell_size) || cell_pos.y
				+ map->cell_size < map->map_area.y
				|| cell_pos.y >= map->map_area.y + (map->cells_y
					* map->cell_size))
				continue ;
			if (data->map->matrix[map_y_int][map_x_int] == 1)
				color = 0xFFFFFF; // Wall
			else if (data->map->matrix[map_y_int][map_x_int] == 2)
				color = 0xEAFA33; // Special object (interruptor)
			else if (data->map->matrix[map_y_int][map_x_int] == 0)
				continue ; // Skip empty space
			else
				color = 0x808080; // Other
			draw_cell(data, cell_pos, color, 0.8);
		}
	}
}

/* Draw player on map (similar to minimap style) */
static void	draw_player(t_data *data, t_show_map *map)
{
	int			player_size;
	t_triangle	t;
	double		dir_length;
	t_coord		pixel;
	double		perp_x;
	double		perp_y;

	player_size = map->cell_size / 2;
	if (player_size < 2)
		player_size = 2;
	// Draw player circle
	for (int y = -player_size; y <= player_size; y++)
	{
		for (int x = -player_size; x <= player_size; x++)
		{
			if (x * x + y * y <= player_size * player_size)
			{
				pixel = {map->player_pos.x + x, map->player_pos.y + y};
				if (pixel.x >= 0 && pixel.x < WIDTH && pixel.y >= 0
					&& pixel.y < HEIGHT)
				{
					draw_pixel(&data->s_img, pixel.x, pixel.y, 0x8b45);
				}
			}
		}
	}
	// Calculate direction for triangle (similar to minimap)
	t.dir_x = data->player.dir_x;
	t.dir_y = data->player.dir_y;
	dir_length = sqrt(t.dir_x * t.dir_x + t.dir_y * t.dir_y) * 0.8;
	if (dir_length > 0)
	{
		t.dir_x /= dir_length;
		t.dir_y /= dir_length;
	}
	// Calculate triangle points
	t.tip_x = map->player_pos.x + (int)(t.dir_x * player_size * 2);
	t.tip_y = map->player_pos.y + (int)(t.dir_y * player_size * 2);
	perp_x = -t.dir_y;
	perp_y = t.dir_x;
	t.left_x = map->player_pos.x + (int)(t.dir_x * player_size * 0.3 + perp_x
			* player_size);
	t.left_y = map->player_pos.y + (int)(t.dir_y * player_size * 0.3 + perp_y
			* player_size);
	t.right_x = map->player_pos.x + (int)(t.dir_x * player_size * 0.3 - perp_x
			* player_size);
	t.right_y = map->player_pos.y + (int)(t.dir_y * player_size * 0.3 - perp_y
			* player_size);
	// Draw the triangle
	draw_line(data, (t_coord){t.tip_x, t.tip_y}, (t_coord){t.left_x, t.left_y},
		0xFF);
	draw_line(data, (t_coord){t.tip_x, t.tip_y}, (t_coord){t.right_x,
		t.right_y}, 0xFF);
}

/* Draw both portals if they're visible on the map */
static void	draw_portals(t_data *data, t_show_map *map)
{
	int		portal_size;
	int		portal_color;
	t_coord	portal_pos;
	double	display_x;
	double	display_y;

	portal_size = map->cell_size / 2;
	if (portal_size < 1)
		portal_size = 1;
	portal_color = 0x00FFFF; // Cyan for both portals
	// Draw portal1 if visible
	if (data->portal1.x != -1)
	{
		// Get the exact position in the display coordinates
		display_x = (data->portal1.x - map->start_map_x) * map->cell_size
			- map->offset_x;
		display_y = (data->portal1.y - map->start_map_y) * map->cell_size
			- map->offset_y;
		// Check if portal is in view area
		if (display_x + map->cell_size >= 0 && display_x < map->cells_x
			* map->cell_size && display_y + map->cell_size >= 0
			&& display_y < map->cells_y * map->cell_size)
		{
			portal_pos.x = map->map_area.x + display_x + (map->cell_size / 2);
			portal_pos.y = map->map_area.y + display_y + (map->cell_size / 2);
			draw_portal_circle(data, portal_pos, portal_size, portal_color);
		}
	}
	// Draw portal2 if visible
	if (data->portal2.x != -1)
	{
		// Get the exact position in the display coordinates
		display_x = (data->portal2.x - map->start_map_x) * map->cell_size
			- map->offset_x;
		display_y = (data->portal2.y - map->start_map_y) * map->cell_size
			- map->offset_y;
		// Check if portal is in view area
		if (display_x + map->cell_size >= 0 && display_x < map->cells_x
			* map->cell_size && display_y + map->cell_size >= 0
			&& display_y < map->cells_y * map->cell_size)
		{
			portal_pos.x = map->map_area.x + display_x + (map->cell_size / 2);
			portal_pos.y = map->map_area.y + display_y + (map->cell_size / 2);
			draw_portal_circle(data, portal_pos, portal_size, portal_color);
		}
	}
}

void	show_map(t_data *data)
{
	t_show_map map;

	draw_texture_centered(data, data->tex[TEX_EMPTY_FRAME], (t_coord){WIDTH
		/ 2.0f, HEIGHT / 2.0f}, 0.6);
	init_show_map(data, &map);
	draw_map_cells(data, &map);
	draw_player(data, &map);
	draw_portals(data, &map);
	draw_texture_centered(data, data->tex[TEX_FRAME_DETAILS], (t_coord){WIDTH
		/ 2 - data->tex[TEX_EMPTY_FRAME]->width / 2
		+ data->tex[TEX_FRAME_DETAILS]->width / 2, HEIGHT / 2
		+ data->tex[TEX_EMPTY_FRAME]->height / 2 + 32}, 0);
}