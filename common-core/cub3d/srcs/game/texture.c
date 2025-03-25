/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 22:25:28 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/25 00:14:41 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	compute_tex(t_data *data, t_raycasting *ray, int x)
{
	int				color;
	unsigned char	*pixel;

	if (ray->side == 0)
		ray->wall.wall_x = data->player.pos_y + ray->wall.wall_distance
			* ray->ray_dir_y;
	else
		ray->wall.wall_x = data->player.pos_x + ray->wall.wall_distance
			* ray->ray_dir_x;
	ray->wall.wall_x -= floor(ray->wall.wall_x);
	ray->wall.tex_x = (int)(ray->wall.wall_x * (double)TEX_WIDTH);
	if (ray->side == 0 && ray->ray_dir_x > 0)
		ray->wall.tex_x = TEX_WIDTH - ray->wall.tex_x - 1;
	if (ray->side == 1 && ray->ray_dir_y < 0)
		ray->wall.tex_x = TEX_WIDTH - ray->wall.tex_x - 1;
	ray->wall.step = 1.0 * TEX_HEIGHT / ray->wall.line_height;
	ray->wall.tex_pos = (ray->wall.draw_start - HEIGHT / 2
			+ ray->wall.line_height / 2) * ray->wall.step;
	ray->wall.tex_index = 0;
	for (int y = ray->wall.draw_start; y < ray->wall.draw_end; y++)
	{
		ray->wall.tex_y = (int)ray->wall.tex_pos & (TEX_HEIGHT - 1);
		ray->wall.tex_pos += ray->wall.step;
		pixel = (unsigned char *)&data->tex[1]->img.addr[(ray->wall.tex_y
				* TEX_WIDTH + ray->wall.tex_x)
			* (data->tex[1]->img.bits_per_pixel / 8)];
		color = (pixel[2] << 16) | (pixel[1] << 8) | pixel[0];
		draw_pixel(&data->s_img, x, y, color);
	}
}
