/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 18:12:39 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/29 19:44:56 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	draw_fps(t_data *data)
{
	char	*tmp;

	tmp = ft_itoa(data->s_fps.fps);
	if (tmp)
	{
		mlx_string_put(data->s_mlx.mlx, data->s_mlx.win, 50, 50, TEXT_COLOR,
			tmp);
		free(tmp);
		tmp = NULL;
	}
}

static void	render_gun(t_data *data)
{
	int	x;
	int	y;
	int	color;
	int	offset_x;
	int	offset_y;

	offset_x = WIDTH / 10;
	offset_y = 200;
	y = -1;
	while (++y < data->gun_tex->height - offset_y)
	{
		x = -1;
		while (++x < data->gun_tex->width)
		{
			color = *(unsigned int *)get_texture_pixel(data->gun_tex, x, y);
			if (((color >> 24) & 0xFF) != 0xFF)
				draw_pixel(&data->s_img, WIDTH - data->gun_tex->width - offset_x
					+ x, HEIGHT - data->gun_tex->height + y + offset_y, color);
		}
	}
}

void	draw_interact(t_data *data)
{
	int				curr_x;
	int				curr_y;
	t_raycasting	ray;

	curr_x = (int)data->player.pos_x;
	curr_y = (int)data->player.pos_y;
	init_ray(data, &ray, WIDTH / 2);
	dda(data, &ray);
	if (abs(curr_x - ray.ray_x) <= 2 && abs(curr_y - ray.ray_y) <= 2)
	{
		if (data->map->matrix[ray.ray_y][ray.ray_x] == 2)
			mlx_string_put(data->s_mlx.mlx, data->s_mlx.win, (WIDTH / 2) - 48,
				(HEIGHT / 2) - POINTER_SIZE - 24, TEXT_COLOR,
				"Press [E] to shut down power.");
	}
}

void	render_start(t_data *data)
{
	int	x_pos;
	int	y_pos;
	int	color;

	x_pos = (WIDTH - data->tex[TEX_START]->width) / 2;
	y_pos = (HEIGHT - data->tex[TEX_START]->height - 64) / 2;
	if (!data->started)
		draw_texture(data, data->tex[TEX_START], (t_coord){x_pos, y_pos}, 0.8);
}

void	render_mission_failed(t_data *data)
{
	int	x_pos;
	int	y_pos;
	int	y;
	int	x;
	int	color;

	x_pos = (WIDTH - data->tex[TEX_MISSION_FAILED]->width) / 2;
	y_pos = (HEIGHT - data->tex[TEX_MISSION_FAILED]->height) / 2;
	if (!data->started)
		draw_texture(data, data->tex[TEX_MISSION_FAILED],
			(t_coord){(double)(WIDTH - data->tex[TEX_MISSION_FAILED]->width)
			/ 2, (double)(HEIGHT - data->tex[TEX_MISSION_FAILED]->height) / 2},
			0);
}

void		show_map(t_data *data);

int	render_hud(t_data *data)
{
	draw_mouse(data);
	render_start(data);
	if (data->game_end)
		draw_texture(data, data->tex[TEX_MISSION_FAILED],
			(t_coord){(double)(WIDTH - data->tex[TEX_MISSION_FAILED]->width)
			/ 2, (double)(HEIGHT - data->tex[TEX_MISSION_FAILED]->height) / 2},
			0.5);
	if (data->started)
		draw_texture(data, data->tex[TEX_SMALL_FRAME], (t_coord){WIDTH
			- data->tex[TEX_SMALL_FRAME]->width - 50, 50}, 1);
	if (data->started)
		render_gun(data);
	if (data->show_map && data->started)
		show_map(data);
	if (!data->show_map && data->started)
		render_minimap(data);
	return (0);
}
