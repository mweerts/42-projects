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

static void	draw_portal(t_data *data, t_fullmap *map, t_portal portal)
{
	int		portal_size;
	t_coord	portal_pos;
	double	display_x;
	double	display_y;

	portal_size = map->cell_size / 2;
	if (portal_size < 1)
		portal_size = 1;
	if (portal.x != -1)
	{
		display_x = (portal.x - map->start.x) * map->cell_size - map->offset_x;
		display_y = (portal.y - map->start.y) * map->cell_size - map->offset_y;
		portal_pos.x = map->map_area.x + display_x + (map->cell_size / 2.0f);
		portal_pos.y = map->map_area.y + display_y + (map->cell_size / 2.0f);
		draw_circle(data, portal_pos, portal_size, MAIN_DARK_PINK);
	}
}

void	show_map(t_data *data)
{
	t_fullmap	map;
	t_coord		pos;

	draw_texture_centered(data, data->tex[TEX_EMPTY_FRAME], (t_coord){WIDTH
		/ 2.0f, HEIGHT / 2.0f}, 0.6);
	init_fullmap(data, &map);
	draw_map_cells(data, &map);
	draw_player(data, &map);
	draw_portal(data, &map, data->portal1);
	draw_portal(data, &map, data->portal2);
	pos.x = WIDTH / 2.0f - data->tex[TEX_EMPTY_FRAME]->width / 2.0f
		+ data->tex[TEX_FRAME_DETAILS]->width / 2.0f;
	pos.y = HEIGHT / 2.0f + data->tex[TEX_EMPTY_FRAME]->height / 2 + 32;
	draw_texture_centered(data, data->tex[TEX_FRAME_DETAILS], pos, 0);
}
