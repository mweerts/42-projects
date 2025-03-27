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

static void	calculate_direction(t_data *data, t_triangle *t, float *dir_length)
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
	float	perp_x;
	float	perp_y;

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
	float		dir_length;

	player_size = minimap->radius / 20.0f;
	if (player_size < 1)
		player_size = 1;
	draw_player_circle(data, minimap, player_size);
	calculate_direction(data, &t, &dir_length);
	calculate_triangle_points(minimap, &t, player_size);
	draw_line(data, (t_coord){t.tip_x, t.tip_y}, (t_coord){t.left_x, t.left_y},
		0xFF);
	draw_line(data, (t_coord){t.tip_x, t.tip_y}, (t_coord){t.right_x,
		t.right_y}, 0xFF);
	return (0);
}

// with simple line
// int render_player_in_minimap(t_data *data, t_minimap *minimap)
// {
//     int     i, j;
//     int     player_size;
//     int     dir_x, dir_y;

//     // Set player marker size
//     player_size = minimap->radius / 20.0f;
//     if (player_size < 1)
//         player_size = 1;

//     // Draw player circle
//     for (i = -player_size; i <= player_size; i++)
//     {
//         for (j = -player_size; j <= player_size; j++)
//         {
//             if (i * i + j * j <= player_size * player_size)
//             {
//                 draw_pixel(&data->s_img, minimap->center_x + j,
//                     minimap->center_y + i, 0x8b45);
//             }
//         }
//     }

//     // Calculate normalized direction vector
//     float normalized_dir_x = data->player.dir_x;
//     float normalized_dir_y = data->player.dir_y;
//     float dir_length = sqrt(normalized_dir_x * normalized_dir_x
//		+ normalized_dir_y * normalized_dir_y);

//     if (dir_length > 0) {
//         normalized_dir_x /= dir_length;
//         normalized_dir_y /= dir_length;
//     }

//     // Draw a simple line showing direction
//     dir_x = minimap->center_x + (int)(normalized_dir_x * player_size * 2);
//     dir_y = minimap->center_y + (int)(normalized_dir_y * player_size * 2);

//     draw_line(data, minimap->center_x, minimap->center_y, dir_x, dir_y,
//	0xFF);

//     return (0);
// }