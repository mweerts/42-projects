/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 03:59:01 by llebugle          #+#    #+#             */
/*   Updated: 2025/03/31 03:59:03 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	calculate_direction(t_data *data, t_triangle *t, double *dir_length)
{
	t->dir_x = data->player.dir_x;
	t->dir_y = data->player.dir_y;
	*dir_length = sqrt(t->dir_x * t->dir_x + t->dir_y * t->dir_y) * 0.8;
	if (*dir_length > 0)
	{
		t->dir_x /= *dir_length;
		t->dir_y /= *dir_length;
	}
}

void	draw_circle(t_data *data, t_coord center, int radius, int color)
{
	int	i;
	int	j;

	i = -radius;
	while (i <= radius)
	{
		j = -radius;
		while (j <= radius)
		{
			if (i * i + j * j <= radius * radius)
				draw_transparent_pixel(data, (t_coord){center.x + j, center.y
					+ i}, color, 0.8);
			j++;
		}
		i++;
	}
}

void	calculate_triangle_points(t_coord center, t_triangle *t, int size)
{
	double	perp_x;
	double	perp_y;

	t->tip_x = center.x + (int)(t->dir_x * size * 2);
	t->tip_y = center.y + (int)(t->dir_y * size * 2);
	perp_x = -t->dir_y;
	perp_y = t->dir_x;
	t->left_x = center.x + (int)(t->dir_x * size * 0.3 + perp_x * size);
	t->left_y = center.y + (int)(t->dir_y * size * 0.3 + perp_y * size);
	t->right_x = center.x + (int)(t->dir_x * size * 0.3 - perp_x * size);
	t->right_y = center.y + (int)(t->dir_y * size * 0.3 - perp_y * size);
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

void	toggle_mouse(t_data *data)
{
	data->mouse_off ^= 1;
	if (data->mouse_off)
		mlx_mouse_show(data->s_mlx.mlx, data->s_mlx.win);
	else
		mlx_mouse_hide(data->s_mlx.mlx, data->s_mlx.win);
}
