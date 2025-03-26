/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 22:25:28 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/26 15:03:03 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static void	set_texture_index(t_raycasting *ray)
{
	if (ray->side == 1)
	{
		if (ray->ray_dir_y >= 0)
			ray->wall.tex_index = TEX_SOUTH;
		else
			ray->wall.tex_index = TEX_NORTH;
	}
	else
	{
		if (ray->ray_dir_x >= 0)
			ray->wall.tex_index = TEX_EAST;
		else
			ray->wall.tex_index = TEX_WEST;
	}
}

static void	draw_tex_column(t_data *data, t_raycasting *ray, int x)
{
	int				y;
	int				color;
	int				tex_i;
	unsigned char	*src;

	y = ray->wall.draw_start;
	tex_i = ray->wall.tex_index;
	while (y < ray->wall.draw_end)
	{
		ray->wall.tex_y = (int)ray->wall.tex_pos
			& (data->tex[ray->wall.tex_index]->height - 1);
		ray->wall.tex_pos += ray->wall.step;
		src = (unsigned char *)&data->tex[tex_i]->img.addr[ray->wall.tex_y
			* data->tex[tex_i]->img.line_length + ray->wall.tex_x
			* (data->tex[tex_i]->img.bits_per_pixel / 8)];
		color = *(unsigned int *)src;
		draw_pixel(&data->s_img, x, y, color);
		y++;
	}
}

void	compute_tex(t_data *data, t_raycasting *ray, int x)
{
	set_texture_index(ray);
	if (ray->side == 0)
		ray->wall.wall_x = data->player.pos_y + ray->wall.wall_distance
			* ray->ray_dir_y;
	else
		ray->wall.wall_x = data->player.pos_x + ray->wall.wall_distance
			* ray->ray_dir_x;
	ray->wall.wall_x -= floor(ray->wall.wall_x);
	ray->wall.tex_x = (int)(ray->wall.wall_x
			* data->tex[ray->wall.tex_index]->width);
	if (ray->side == 0 && ray->ray_dir_x > 0)
		ray->wall.tex_x = data->tex[ray->wall.tex_index]->width
			- ray->wall.tex_x - 1;
	if (ray->side == 1 && ray->ray_dir_y < 0)
		ray->wall.tex_x = data->tex[ray->wall.tex_index]->width
			- ray->wall.tex_x - 1;
	ray->wall.step = 1.0 * data->tex[ray->wall.tex_index]->height
		/ ray->wall.line_height;
	ray->wall.tex_pos = (ray->wall.draw_start - HEIGHT / 2
			+ ray->wall.line_height / 2) * ray->wall.step;
	draw_tex_column(data, ray, x);
}
