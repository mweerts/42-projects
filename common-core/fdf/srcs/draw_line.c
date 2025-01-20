/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 04:01:17 by maxweert          #+#    #+#             */
/*   Updated: 2025/01/18 04:11:10 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static int	interpolate(int start, int end, float t)
{
	return ((int)(start + (end - start) * t));
}

static int	color_interpolate(int color1, int color2, float t)
{
	int	red;
	int	green;
	int	blue;

	red = interpolate((color1 >> 16) & 0xFF, (color2 >> 16) & 0xFF, t);
	green = interpolate((color1 >> 8) & 0xFF, (color2 >> 8) & 0xFF, t);
	blue = interpolate(color1 & 0xFF, color2 & 0xFF, t);
	return (red << 16 | green << 8 | blue);
}

void	img_line(t_data *data, t_coord p1, t_coord p2)
{
	t_line	line;
	int		i;
	float	tmp;

	line.dx = p2.x - p1.x;
	line.dy = p2.y - p1.y;
	if (abs(line.dx) > abs(line.dy))
		line.steps = abs(line.dx);
	else
		line.steps = abs(line.dy);
	line.x_inc = line.dx / (float)line.steps;
	line.y_inc = line.dy / (float)line.steps;
	i = 0;
	while (i <= line.steps)
	{
		tmp = i / (float)line.steps;
		line.color = color_interpolate(p1.color, p2.color, tmp);
		img_pixel(data, p1.x + i * line.x_inc, p1.y
			+ i * line.y_inc, line.color);
		i++;
	}
}
