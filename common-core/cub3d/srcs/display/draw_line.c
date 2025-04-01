/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 22:30:14 by llebugle          #+#    #+#             */
/*   Updated: 2025/03/27 22:30:17 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static void	init_line_params(t_line *line, t_coord start, t_coord end)
{
	line->current = start;
	line->end = end;
	line->dx = abs(end.x - start.x);
	line->dy = -(abs(end.y - start.y));
	line->sx = -1;
	if (start.x < end.x)
		line->sx = 1;
	line->sy = -1;
	if (start.y < end.y)
		line->sy = 1;
	line->err = line->dx + line->dy;
}

static void	update_horizontal_pos(t_line *line)
{
	if (line->e2 >= line->dy)
	{
		if (line->current.x == line->end.x)
			line->done = 1;
		else
		{
			line->err += line->dy;
			line->current.x += line->sx;
		}
	}
}

static void	update_vertical_pos(t_line *line)
{
	if (line->e2 <= line->dx)
	{
		if (line->current.y == line->end.y)
			line->done = 1;
		else
		{
			line->err += line->dx;
			line->current.y += line->sy;
		}
	}
}

void	draw_line(t_data *data, t_coord start, t_coord end, int color)
{
	t_line	line;

	init_line_params(&line, start, end);
	line.done = 0;
	while (!line.done)
	{
		draw_pixel(&data->s_img, line.current.x, line.current.y, color);
		if (line.current.x == line.end.x && line.current.y == line.end.y)
			break ;
		line.e2 = 2 * line.err;
		update_horizontal_pos(&line);
		update_vertical_pos(&line);
	}
}
