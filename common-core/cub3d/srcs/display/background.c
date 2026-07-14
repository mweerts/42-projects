/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   background.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 17:03:56 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/29 22:09:20 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	set_background(t_data *data)
{
	int	x;
	int	y;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			if (y > (HEIGHT / 2))
				draw_pixel(&data->s_img, x, y, data->map->floor_color);
			else
				draw_pixel(&data->s_img, x, y, data->map->ceiling_color);
			x++;
		}
		y++;
	}
}

static void	set_floor_and_ceiling_pixel(t_data *data, int x, int y,
		t_ray_floor *ray)
{
	int				tx;
	int				ty;
	unsigned char	*src;
	int				color;

	tx = (int)(data->tex[TEX_FLOOR]->width * (ray->floor_x
				- (int)ray->floor_x)) & (data->tex[TEX_FLOOR]->width - 1);
	ty = (int)(data->tex[TEX_FLOOR]->height * (ray->floor_y
				- (int)ray->floor_y)) & (data->tex[TEX_FLOOR]->height - 1);
	src = (unsigned char *)&data->tex[TEX_FLOOR]->img.addr[ty
		* data->tex[TEX_FLOOR]->img.line_length + tx
		* (data->tex[TEX_FLOOR]->img.bits_per_pixel / 8)];
	color = *(unsigned int *)src;
	draw_pixel(&data->s_img, x, y, color);
	draw_pixel(&data->s_img, x, HEIGHT - y - 1, 0x0);
}

static void	compute_row_data(t_data *data, int y, t_ray_floor *ray)
{
	float	ray_dir_x0;
	float	ray_dir_y0;
	float	ray_dir_x1;
	float	ray_dir_y1;
	int		p;

	ray_dir_x0 = data->player.dir_x - data->player.plane_x;
	ray_dir_y0 = data->player.dir_y - data->player.plane_y;
	ray_dir_x1 = data->player.dir_x + data->player.plane_x;
	ray_dir_y1 = data->player.dir_y + data->player.plane_y;
	p = y - HEIGHT / 2;
	ray->pos_z = 0.5 * HEIGHT;
	ray->row_distance = ray->pos_z / p;
	ray->floor_step_x = ray->row_distance * (ray_dir_x1 - ray_dir_x0) / WIDTH;
	ray->floor_step_y = ray->row_distance * (ray_dir_y1 - ray_dir_y0) / WIDTH;
	ray->floor_x = data->player.pos_x + ray->row_distance * ray_dir_x0;
	ray->floor_y = data->player.pos_y + ray->row_distance * ray_dir_y0;
}

static void	cast_floor_rays(t_data *data, int y)
{
	int			x;
	t_ray_floor	ray;

	compute_row_data(data, y, &ray);
	x = 0;
	while (x < WIDTH)
	{
		set_floor_and_ceiling_pixel(data, x, y, &ray);
		ray.floor_x += ray.floor_step_x;
		ray.floor_y += ray.floor_step_y;
		x++;
	}
}

void	set_textured_background(t_data *data)
{
	int	y;

	y = HEIGHT / 2;
	while (y < HEIGHT)
	{
		cast_floor_rays(data, y);
		y++;
	}
}
