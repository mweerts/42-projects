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

static void	calc_map_dimensions(t_data *data, t_coord *map_area, int *cells_x,
		int *cells_y)
{
	int		frame_width;
	int		frame_height;
	int		map_area_width;
	int		map_area_height;
	t_coord	frame_center;

	frame_width = data->tex[TEX_EMPTY_FRAME]->width;
	frame_height = data->tex[TEX_EMPTY_FRAME]->height;
	map_area_width = frame_width - (FRAME_MARGIN * 2);
	map_area_height = frame_height - (FRAME_MARGIN * 2);
	*cells_x = map_area_width / MAP_CELL_SIZE;
	*cells_y = map_area_height / MAP_CELL_SIZE;
	frame_center.x = WIDTH / 2;
	frame_center.y = HEIGHT / 2;
	map_area->x = frame_center.x - (map_area_width / 2.0f);
	map_area->y = frame_center.y - (map_area_height / 2.0f) - 10;
}

static void	draw_cell(t_data *data, t_coord cell_pos, int color, double alpha)
{
	int		x;
	int		y;
	t_coord	pixel;

	y = 0;
	while (y < MAP_CELL_SIZE)
	{
		x = 0;
		while (x < MAP_CELL_SIZE)
		{
			pixel.x = cell_pos.x + x;
			pixel.y = cell_pos.y + y;
			if (pixel.x >= 0 && pixel.x < WIDTH && pixel.y >= 0
				&& pixel.y < HEIGHT)
				draw_transparent_pixel(data, pixel, color, alpha);
			x++;
		}
		y++;
	}
}

/* Draw map cells based on map content - with fluid movement */
static void	draw_map_cells(t_data *data, t_coord map_area, int cells_x,
		int cells_y, t_coord start)
{
	int		x;
	int		y;
	int		map_x_int;
	int		map_y_int;
	double	offset_x;
	double	offset_y;
	t_coord	cell_pos;
	int		color;

	// Calculate fractional offsets for smooth movement
	offset_x = (start.x - floor(start.x)) * MAP_CELL_SIZE;
	offset_y = (start.y - floor(start.y)) * MAP_CELL_SIZE;
	// Adjust start position to whole numbers but keep track of the fraction
	start.x = floor(start.x);
	start.y = floor(start.y);
	// We need one extra cell in each direction to handle scrolling
	for (y = -1; y < cells_y + 1; y++)
	{
		map_y_int = (int)start.y + y;
		if (!is_in_map_bounds(data, 0, map_y_int))
			continue ;
		for (x = -1; x < cells_x + 1; x++)
		{
			map_x_int = (int)start.x + x;
			if (!is_in_map_bounds(data, map_x_int, map_y_int))
				continue ;
			// Apply fractional offset for smooth scrolling
			cell_pos.x = map_area.x + (x * MAP_CELL_SIZE) - offset_x;
			cell_pos.y = map_area.y + (y * MAP_CELL_SIZE) - offset_y;
			// Skip cells that would be completely outside the visible area
			if (cell_pos.x + MAP_CELL_SIZE < map_area.x
				|| cell_pos.x >= map_area.x + (cells_x * MAP_CELL_SIZE)
				|| cell_pos.y + MAP_CELL_SIZE < map_area.y
				|| cell_pos.y >= map_area.y + (cells_y * MAP_CELL_SIZE))
				continue ;
			if (data->map->matrix[map_y_int][map_x_int] == 1)
				color = 0xFFFFFF; // Wall
			else if (data->map->matrix[map_y_int][map_x_int] == 2
				|| data->map->matrix[map_y_int][map_x_int] == -2)
				color = 0x00FFFF; // Special object (interruptor)
			else if (data->map->matrix[map_y_int][map_x_int] == 0)
				continue ; // Skip empty space
			else
				color = 0x808080; // Other
			draw_cell(data, cell_pos, color, 0.8);
		}
	}
}

/* Draw player on map (similar to minimap style) */
static void	draw_player(t_data *data, t_coord player_pos, int cell_size)
{
	int			player_size;
	t_triangle	t;
	double		dir_length;
	t_coord		pixel;
	double		perp_x;
	double		perp_y;

	player_size = cell_size / 2;
	if (player_size < 2)
		player_size = 2;
	// Draw player circle
	for (int y = -player_size; y <= player_size; y++)
	{
		for (int x = -player_size; x <= player_size; x++)
		{
			if (x * x + y * y <= player_size * player_size)
			{
				pixel = {player_pos.x + x, player_pos.y + y};
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
	t.tip_x = player_pos.x + (int)(t.dir_x * player_size * 2);
	t.tip_y = player_pos.y + (int)(t.dir_y * player_size * 2);
	perp_x = -t.dir_y;
	perp_y = t.dir_x;
	t.left_x = player_pos.x + (int)(t.dir_x * player_size * 0.3 + perp_x
			* player_size);
	t.left_y = player_pos.y + (int)(t.dir_y * player_size * 0.3 + perp_y
			* player_size);
	t.right_x = player_pos.x + (int)(t.dir_x * player_size * 0.3 - perp_x
			* player_size);
	t.right_y = player_pos.y + (int)(t.dir_y * player_size * 0.3 - perp_y
			* player_size);
	// Draw the triangle
	draw_line(data, (t_coord){t.tip_x, t.tip_y}, (t_coord){t.left_x, t.left_y},
		0xFF);
	draw_line(data, (t_coord){t.tip_x, t.tip_y}, (t_coord){t.right_x,
		t.right_y}, 0xFF);
}

/* Draw both portals if they're visible on the map - with fluid movement */
static void	draw_portals(t_data *data, t_coord map_area, int cells_x,
		int cells_y, t_coord start)
{
	int		portal_size;
	int		portal_color;
	t_coord	portal_pos;
	double	offset_x;
	double	offset_y;
	double	display_x;
	double	display_y;

	portal_size = MAP_CELL_SIZE / 2;
	if (portal_size < 1)
		portal_size = 1;
	portal_color = 0x00FFFF; // Cyan for both portals
	// Calculate fractional offsets for smooth movement
	offset_x = (start.x - floor(start.x)) * MAP_CELL_SIZE;
	offset_y = (start.y - floor(start.y)) * MAP_CELL_SIZE;
	// Draw portal1 if visible
	if (data->portal1.x != -1)
	{
		// Get the exact position in the display coordinates
		display_x = (data->portal1.x - floor(start.x)) * MAP_CELL_SIZE
			- offset_x;
		display_y = (data->portal1.y - floor(start.y)) * MAP_CELL_SIZE
			- offset_y;
		// Check if portal is in view area
		if (display_x + MAP_CELL_SIZE >= 0 && display_x < cells_x
			* MAP_CELL_SIZE && display_y + MAP_CELL_SIZE >= 0
			&& display_y < cells_y * MAP_CELL_SIZE)
		{
			portal_pos.x = map_area.x + display_x + (MAP_CELL_SIZE / 2.0f);
			portal_pos.y = map_area.y + display_y + (MAP_CELL_SIZE / 2.0f);
			draw_portal_circle(data, portal_pos, portal_size, portal_color);
		}
	}
	// Draw portal2 if visible
	if (data->portal2.x != -1)
	{
		// Get the exact position in the display coordinates
		display_x = (data->portal2.x - floor(start.x)) * MAP_CELL_SIZE
			- offset_x;
		display_y = (data->portal2.y - floor(start.y)) * MAP_CELL_SIZE
			- offset_y;
		// Check if portal is in view area
		if (display_x + MAP_CELL_SIZE >= 0 && display_x < cells_x
			* MAP_CELL_SIZE && display_y + MAP_CELL_SIZE >= 0
			&& display_y < cells_y * MAP_CELL_SIZE)
		{
			portal_pos.x = map_area.x + display_x + (MAP_CELL_SIZE / 2.0f);
			portal_pos.y = map_area.y + display_y + (MAP_CELL_SIZE / 2.0f);
			draw_portal_circle(data, portal_pos, portal_size, portal_color);
		}
	}
}

void	render_map(t_data *data)
{
	t_coord	map_area;
	t_coord	player_pos;
	int		cells_x;
	int		cells_y;
	t_coord	start;

	calc_map_dimensions(data, &map_area, &cells_x, &cells_y);
	start.x = data->player.pos_x - (cells_x / 2.0);
	start.y = data->player.pos_y - (cells_y / 2.0);
	draw_map_cells(data, map_area, cells_x, cells_y, start);
	player_pos.x = map_area.x + (cells_x / 2.0) * MAP_CELL_SIZE;
	player_pos.y = map_area.y + (cells_y / 2.0) * MAP_CELL_SIZE;
	draw_player(data, player_pos, MAP_CELL_SIZE);
	draw_portals(data, map_area, cells_x, cells_y, start);
}

void	show_map(t_data *data)
{
	draw_texture_centered(data, data->tex[TEX_EMPTY_FRAME], (t_coord){WIDTH
		/ 2.0f, HEIGHT / 2.0f}, 0.6);
	render_map(data);
	draw_texture_centered(data, data->tex[TEX_FRAME_DETAILS], (t_coord){WIDTH
		/ 2 - data->tex[TEX_EMPTY_FRAME]->width / 2
		+ data->tex[TEX_FRAME_DETAILS]->width / 2, HEIGHT / 2
		+ data->tex[TEX_EMPTY_FRAME]->height / 2 + 32}, 0);
}