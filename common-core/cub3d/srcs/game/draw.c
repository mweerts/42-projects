/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:43:07 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/23 20:37:20 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	draw_pixel(t_img *s_img, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x > WIDTH || y < 0 || y > HEIGHT)
		return (0);
	dst = s_img->addr + (y * s_img->line_length
			+ x * (s_img->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
	return (1);
}

int	draw_line(t_img *s_img, t_screen_coord p1, t_screen_coord p2, int color)
{
	int		dx;
	int		dy;
	int		steps;
	int		i;

	dx = p2.x - p1.x;
	dy = p2.y - p1.y;
	if (abs(dx) > abs(dy))
		steps = abs(dx);
	else
		steps = abs(dy);
	i = 0;
	while (i <= steps)
	{
		if (!draw_pixel(s_img, p1.x + i * (dx / (float)steps), p1.y
				+ i * (dy / (float)steps), color))
			return (0);
		i++;
	}
	return (1);
}

int	draw_vertical_line(t_img *s_img, int x, int start, int stop, int color)
{
	while (start <= stop)
	{
		if (!draw_pixel(s_img, x, start, color))
			return (0);
		start++;
	}
	return (1);
}
