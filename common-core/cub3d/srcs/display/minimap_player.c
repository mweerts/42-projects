/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_player.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 22:39:34 by llebugle          #+#    #+#             */
/*   Updated: 2025/03/27 22:39:35 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static void	calculate_direction(t_data *data, t_triangle *t, double *dir_length)
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

static void	calculate_triangle_points(t_minimap *minimap, t_triangle *t, int ps)
{
	double	perp_x;
	double	perp_y;

	t->tip_x = minimap->center_x + (int)(t->dir_x * ps * 2);
	t->tip_y = minimap->center_y + (int)(t->dir_y * ps * 2);
	perp_x = -t->dir_y;
	perp_y = t->dir_x;

	t->left_x = minimap->center_x + (int)(t->dir_x * ps * 0.3 + perp_x * ps);
	t->left_y = minimap->center_y + (int)(t->dir_y * ps * 0.3 + perp_y * ps);
	t->right_x = minimap->center_x + (int)(t->dir_x * ps * 0.3 - perp_x * ps);
	t->right_y = minimap->center_y + (int)(t->dir_y * ps * 0.3 - perp_y * ps);
}


static void	draw_player_circle(t_data *data, t_minimap *minimap,
		int player_size)
{
	int	i;
	int	j;

	i = -player_size;
	while (i <= player_size)
	{
		j = -player_size;
		while (j <= player_size)
		{
			if (i * i + j * j <= player_size * player_size)
			{
				draw_pixel(&data->s_img, minimap->center_x + j,
					minimap->center_y + i, 0x8b45);
			}
			j++;
		}
		i++;
	}
}

int	render_player_in_minimap(t_data *data, t_minimap *minimap)
{
	int			player_size;
	t_triangle	t;
	double		dir_length;

	player_size = minimap->radius / 20.0f;
	if (player_size < 1.0f)
		player_size = 1.0f;
	draw_player_circle(data, minimap, player_size);
	calculate_direction(data, &t, &dir_length);
	calculate_triangle_points(minimap, &t, player_size);
	draw_line(data, (t_coord){t.tip_x, t.tip_y}, (t_coord){t.left_x, t.left_y},
		0xFF);
	draw_line(data, (t_coord){t.tip_x, t.tip_y}, (t_coord){t.right_x,
		t.right_y}, 0xFF);
	return (0);
}
