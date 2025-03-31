/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_mouse_pointer.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 21:54:50 by llebugle          #+#    #+#             */
/*   Updated: 2025/03/31 21:54:52 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static void	draw_halo_circle(t_data *data, t_coord center, int radius)
{
	float	angle;
	t_coord	pos;

	angle = 0.0f;
	while (angle < 2 * M_PI)
	{
		pos.x = center.x + cos(angle) * radius;
		pos.y = center.y + sin(angle) * radius;
		draw_pixel(&data->s_img, pos.x, pos.y, POINTER_COLOR);
		angle += 0.01f;
	}
	draw_pixel(&data->s_img, center.x, center.y, POINTER_COLOR);
}

static void	draw_halo_markers(t_data *data, t_coord center, int radius,
		int marker_length)
{
	int		i;
	float	angle;
	t_coord	start;
	t_coord	end;
	int		marker_gap;

	marker_gap = 2;
	i = 0;
	while (i < 4)
	{
		angle = i * (2 * M_PI / 4);
		start.x = (int)(center.x + cos(angle) * (radius + marker_gap));
		start.y = (int)(center.y + sin(angle) * (radius + marker_gap));
		end.x = (int)(center.x + cos(angle) * (radius + marker_gap
					+ marker_length));
		end.y = (int)(center.y + sin(angle) * (radius + marker_gap
					+ marker_length));
		draw_line(data, start, end, POINTER_COLOR);
		i++;
	}
}

void	draw_mouse(t_data *data)
{
	t_coord	center;
	int		radius;
	int		marker_length;

	center.x = (int)(WIDTH / 2);
	center.y = (int)(HEIGHT / 2);
	radius = CROSS_SIZE + 2;
	marker_length = radius / 2;
	draw_halo_circle(data, center, radius);
	draw_halo_markers(data, center, radius, marker_length);
}
