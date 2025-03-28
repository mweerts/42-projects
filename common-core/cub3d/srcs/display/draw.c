/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:43:07 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/28 17:56:31 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static void	draw_interact(t_data *data)
{
	int				curr_x;
	int				curr_y;
	t_raycasting	ray;

	curr_x = (int)data->player.pos_x;
	curr_y = (int)data->player.pos_y;
	init_ray(data, &ray, WIDTH / 2);
	dda(data, &ray);
	if (abs(curr_x - ray.ray_x) <= 1 && abs(curr_y - ray.ray_y) <= 1)
	{
		if (data->map->matrix[ray.ray_y][ray.ray_x] == 2)
			mlx_string_put(data->s_mlx.mlx, data->s_mlx.win, (WIDTH / 2) - 75,
				(HEIGHT / 2) - CROSS_SIZE, TEXT_COLOR,
				"Press [E] to open portal.");
		if (data->map->matrix[ray.ray_y][ray.ray_x] == -2)
			mlx_string_put(data->s_mlx.mlx, data->s_mlx.win, (WIDTH / 2) - 75,
				(HEIGHT / 2) - CROSS_SIZE, TEXT_COLOR,
				"Press [E] to close portal.");
	}
}

int	draw_game(t_data *data)
{
	if (BONUS)
		set_textured_background(data);
	else
		set_background(data);
	raycasting(data);
	count_fps(data);
	render_hud(data);
	mlx_put_image_to_window(data->s_mlx.mlx, data->s_mlx.win, data->s_img.img,
		0, 0);
	draw_fps(data);
	draw_interact(data);
	return (1);
}

void	draw_pixel(t_img *s_img, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x > WIDTH || y < 0 || y > HEIGHT)
		return ;
	dst = s_img->addr + (y * s_img->line_length + x * (s_img->bits_per_pixel
				/ 8));
	*(unsigned int *)dst = color;
}

void	draw_pixel_light(t_img *s_img, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x > WIDTH || y < 0 || y > HEIGHT)
		return ;
	dst = s_img->addr + (y * s_img->line_length + x * (s_img->bits_per_pixel
				/ 8));
	*(unsigned int *)dst = color / 8;
}

int	blend_color(int background, int overlay, float alpha)
{
	t_rgb			bg;
	t_rgb			ov;
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;

	bg.r = (background >> 16) & 0xFF;
	bg.g = (background >> 8);
	bg.b = background & 0xFF;
	ov.r = (overlay >> 16) & 0xFF;
	ov.g = (overlay >> 8);
	ov.b = overlay & 0xFF;
	r = (unsigned char)(bg.r * (1 - alpha) + ov.r * alpha);
	g = (unsigned char)(bg.g * (1 - alpha) + ov.g * alpha);
	b = (unsigned char)(bg.b * (1 - alpha) + ov.b * alpha);
	return ((r << 16) | (g << 8) | b);
}

void	draw_transparent_pixel(t_data *data, t_coord coord, int color,
		float alpha)
{
	char	*dst;
	int		background;

	if (coord.x < 0 || coord.x >= WIDTH || coord.y < 0 || coord.y >= HEIGHT)
		return ;
	dst = data->s_img.addr + ((int)coord.y * data->s_img.line_length + (int)coord.x
			* (data->s_img.bits_per_pixel / 8));
	background = *(unsigned int *)dst;
	*(unsigned int *)dst = blend_color(background, color, alpha);
}

