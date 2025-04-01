/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 22:25:28 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/29 19:33:37 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static void	set_texture_index1(t_data *data, t_raycasting *ray)
{
	if (ray->ray_dir_y >= 0)
	{
		if ((is_portal(data, ray->ray_x, ray->ray_y, 1)
				&& data->portal1.orientation == SOUTH) || (is_portal(data,
					ray->ray_x, ray->ray_y, 2)
				&& data->portal2.orientation == SOUTH))
			ray->wall.is_portal = 1;
		ray->wall.tex = data->tex[TEX_NORTH];
	}
	else
	{
		if ((is_portal(data, ray->ray_x, ray->ray_y, 1)
				&& data->portal1.orientation == NORTH) || (is_portal(data,
					ray->ray_x, ray->ray_y, 2)
				&& data->portal2.orientation == NORTH))
			ray->wall.is_portal = 1;
		ray->wall.tex = data->tex[TEX_SOUTH];
	}
}

static void	set_texture_index2(t_data *data, t_raycasting *ray)
{
	if (ray->ray_dir_x >= 0)
	{
		if ((is_portal(data, ray->ray_x, ray->ray_y, 1)
				&& data->portal1.orientation == EAST) || (is_portal(data,
					ray->ray_x, ray->ray_y, 2)
				&& data->portal2.orientation == EAST))
			ray->wall.is_portal = 1;
		ray->wall.tex = data->tex[TEX_WEST];
	}
	else
	{
		if ((is_portal(data, ray->ray_x, ray->ray_y, 1)
				&& data->portal1.orientation == WEST) || (is_portal(data,
					ray->ray_x, ray->ray_y, 2)
				&& data->portal2.orientation == WEST))
			ray->wall.is_portal = 1;
		ray->wall.tex = data->tex[TEX_EAST];
	}
}

static void	draw_tex_column(t_data *data, t_raycasting *ray, int x)
{
	int	y;
	int	color;
	int	tmp;

	y = ray->wall.draw_start;
	while (y < ray->wall.draw_end)
	{
		ray->wall.tex_y = (int)ray->wall.tex_pos & (ray->wall.tex->height - 1);
		ray->wall.tex_pos += ray->wall.step;
		if (ray->wall.is_portal)
			tmp = *(unsigned int *)get_texture_pixel(data->anim.curr_frame,
					ray->wall.tex_x / (ray->wall.tex->width
						/ data->anim.curr_frame->width), ray->wall.tex_y
					/ (ray->wall.tex->height / data->anim.curr_frame->height));
		if (ray->wall.is_portal && ((tmp >> 24) & 0xFF) != 0xFF)
			draw_pixel(&data->s_img, x, y, tmp);
		else
		{
			color = *(unsigned int *)get_texture_pixel(ray->wall.tex,
					ray->wall.tex_x, ray->wall.tex_y);
			draw_pixel(&data->s_img, x, y, color);
		}
		y++;
	}
}

void	compute_tex(t_data *data, t_raycasting *ray, int x)
{
	ray->wall.is_portal = 0;
	if (data->map->matrix[ray->ray_y][ray->ray_x] == 2)
		ray->wall.tex = data->tex[TEX_CORE_DANGER];
	else if (data->map->matrix[ray->ray_y][ray->ray_x] == 3)
		ray->wall.tex = data->tex[TEX_CORE];
	else if (ray->side == 1)
		set_texture_index1(data, ray);
	else
		set_texture_index2(data, ray);
	if (ray->side == 0)
		ray->wall.wall_x = data->player.pos_y + ray->wall.wall_distance
			* ray->ray_dir_y;
	else
		ray->wall.wall_x = data->player.pos_x + ray->wall.wall_distance
			* ray->ray_dir_x;
	ray->wall.wall_x -= floor(ray->wall.wall_x);
	ray->wall.tex_x = (int)(ray->wall.wall_x * (double)ray->wall.tex->width);
	if ((ray->side == 1 && ray->ray_dir_y < 0) || (ray->side == 0
			&& ray->ray_dir_x > 0))
		ray->wall.tex_x = ray->wall.tex->width - ray->wall.tex_x - 1;
	ray->wall.step = 1.0 * ray->wall.tex->height / ray->wall.line_height;
	ray->wall.tex_pos = (ray->wall.draw_start - HEIGHT / 2
			+ ray->wall.line_height / 2) * ray->wall.step;
	draw_tex_column(data, ray, x);
}
