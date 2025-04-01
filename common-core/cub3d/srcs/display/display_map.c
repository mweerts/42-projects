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
static int	is_in_map_bounds(t_data *data, int x, int y)
{
	return (x >= 0 && x < data->map->width && y >= 0 && y < data->map->height);
}

static void	init_fullmap(t_data *data, t_fullmap *map)
{
	int		map_area_width;
	int		map_area_height;
	t_coord	frame_center;

	map->cell_size = MAP_CELL_SIZE;
	map_area_width = data->tex[TEX_EMPTY_FRAME]->width - (FRAME_MARGIN * 2);
	map_area_height = data->tex[TEX_EMPTY_FRAME]->height - (FRAME_MARGIN * 2
			+ 20);
	map->cells_x = map_area_width / map->cell_size;
	map->cells_y = map_area_height / map->cell_size;
	frame_center.x = WIDTH / 2.0f;
	frame_center.y = HEIGHT / 2.0f;
	map->map_area.x = frame_center.x - (map_area_width / 2.0f);
	map->map_area.y = frame_center.y - (map_area_height / 2.0f) - 10;
	map->start.x = data->player.pos_x - (map->cells_x / 2.0);
	map->start.y = data->player.pos_y - (map->cells_y / 2.0);
	map->offset_x = (map->start.x - floor(map->start.x)) * map->cell_size;
	map->offset_y = (map->start.y - floor(map->start.y)) * map->cell_size;
	map->start.x = floor(map->start.x);
	map->start.y = floor(map->start.y);
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
static void	draw_map_cells(t_data *data, t_fullmap *map)
{
	int		x;
	int		y;
	int		map_x_int;
	int		map_y_int;
	t_coord	cell_pos;
	int		color;

	y = -2;
	// We need one extra cell in each direction to handle scrolling
	while (++y < map->cells_y + 1)
	{
		map_y_int = (int)map->start.y + y;
		if (!is_in_map_bounds(data, 0, map_y_int))
			continue ;
		for (x = -1; x < map->cells_x + 1; x++)
		{
			map_x_int = (int)map->start.x + x;
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
	y++;
}

static void	draw_player(t_data *data, t_fullmap *map)
{
	int			player_size;
	t_triangle	t;
	double		dir_length;

	player_size = map->cell_size / 2;
	if (player_size < 2)
		player_size = 2;
	draw_circle(data, (t_coord){map->player_pos.x, map->player_pos.y},
		player_size, 0x8b45);
	calculate_direction(data, &t, &dir_length);
	calculate_triangle_points((t_coord){map->player_pos.x, map->player_pos.y},
		&t, player_size);
	draw_line(data, (t_coord){t.tip_x, t.tip_y}, (t_coord){t.left_x, t.left_y},
		0xFF);
	draw_line(data, (t_coord){t.tip_x, t.tip_y}, (t_coord){t.right_x,
		t.right_y}, 0xFF);
}

static void	draw_portals(t_data *data, t_fullmap *map)
{
	int		portal_size;
	t_coord	portal_pos;
	double	display_x;
	double	display_y;

	portal_size = map->cell_size / 2;
	if (portal_size < 1)
		portal_size = 1;
	if (data->portal1.x != -1)
	{
		display_x = (data->portal1.x - map->start.x) * map->cell_size
			- map->offset_x;
		display_y = (data->portal1.y - map->start.y) * map->cell_size
			- map->offset_y;
		portal_pos.x = map->map_area.x + display_x + (map->cell_size / 2);
		portal_pos.y = map->map_area.y + display_y + (map->cell_size / 2);
		draw_circle(data, portal_pos, portal_size, MAIN_DARK_PINK);
	}
	if (data->portal2.x != -1)
	{
		display_x = (data->portal2.x - map->start.x) * map->cell_size
			- map->offset_x;
		display_y = (data->portal2.y - map->start.y) * map->cell_size
			- map->offset_y;
		portal_pos.x = map->map_area.x + display_x + (map->cell_size / 2);
		portal_pos.y = map->map_area.y + display_y + (map->cell_size / 2);
		draw_circle(data, portal_pos, portal_size, MAIN_DARK_PINK);
	}
}

void	show_map(t_data *data)
{
	t_fullmap	map;

	draw_texture_centered(data, data->tex[TEX_EMPTY_FRAME], (t_coord){WIDTH
		/ 2.0f, HEIGHT / 2.0f}, 0.6);
	init_fullmap(data, &map);
	draw_map_cells(data, &map);
	draw_player(data, &map);
	draw_portals(data, &map);
	draw_texture_centered(data, data->tex[TEX_FRAME_DETAILS], (t_coord){WIDTH
		/ 2 - data->tex[TEX_EMPTY_FRAME]->width / 2
		+ data->tex[TEX_FRAME_DETAILS]->width / 2, HEIGHT / 2
		+ data->tex[TEX_EMPTY_FRAME]->height / 2 + 32}, 0);
}

