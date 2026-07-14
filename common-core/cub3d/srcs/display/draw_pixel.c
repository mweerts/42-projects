/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_pixel.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 03:57:47 by llebugle          #+#    #+#             */
/*   Updated: 2025/03/31 03:57:51 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

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

void	draw_transparent_pixel(t_data *data, t_coord coord, int color,
		float alpha)
{
	char	*dst;
	int		background;

	if (coord.x < 0 || coord.x >= WIDTH || coord.y < 0 || coord.y >= HEIGHT)
		return ;
	dst = data->s_img.addr + ((int)coord.y * data->s_img.line_length
			+ (int)coord.x * (data->s_img.bits_per_pixel / 8));
	background = *(unsigned int *)dst;
	*(unsigned int *)dst = blend_color(background, color, alpha);
}
