/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 15:01:55 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/24 16:56:13 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static void	init_side_and_step(t_data *data, t_raycasting *ray)
{
	if (ray->ray_dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_dist_x = (data->player.pos_x - ray->ray_x)
			* ray->delta_dist_x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist_x = (ray->ray_x + 1.0 - data->player.pos_x)
			* ray->delta_dist_x;
	}
	if (ray->ray_dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (data->player.pos_y - ray->ray_y)
			* ray->delta_dist_y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist_y = (ray->ray_y + 1.0 - data->player.pos_y)
			* ray->delta_dist_y;
	}
}

static void	init_ray(t_data *data, t_raycasting *ray, int x)
{
	ray->camera_x = 2 * x / (double)data->map->width - 1;
	ray->ray_dir_x = data->player.dir_x + data->player.plane_x * ray->camera_x;
	ray->ray_dir_y = data->player.dir_y + data->player.plane_y * ray->camera_x;
	ray->ray_x = (int)data->player.pos_x;
	ray->ray_y = (int)data->player.pos_y;
	ray->delta_dist_x = fabs(1 / ray->ray_dir_x);
	ray->delta_dist_y = fabs(1 / ray->ray_dir_y);
	ray->hit = 0;
	init_side_and_step(data, ray);
}

static void	dda(t_data *data, t_raycasting *ray)
{
	while (ray->hit == 0)
	{
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->side_dist_x += ray->delta_dist_x;
			ray->ray_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist_y += ray->delta_dist_y;
			ray->ray_y += ray->step_y;
			ray->side = 1;
		}
		if (data->map->matrix[ray->ray_y][ray->ray_x] > 0)
			ray->hit = 1;
	}
	if (ray->side == 0)
		ray->wall_distance = ray->side_dist_x - ray->delta_dist_x;
	else
		ray->wall_distance = ray->side_dist_y - ray->delta_dist_y;
}

int	raycasting(t_data *data)
{
	t_raycasting	ray;
	int				x;
	int				draw_start;
	int				draw_end;
	int				line_height;

	set_background(data);
	compute_player_pos(data);
	x = 0;
	while (x < data->map->height)
	{
		init_ray(data, &ray, x);
		dda(data, &ray);
		line_height = (int)(data->map->width / ray.wall_distance);
		if (line_height < 0)
			line_height = data->map->width;
		draw_start = -line_height / 2 + data->map->width / 2;
		if (draw_start < 0)
			draw_start = 0;
		draw_end = line_height / 2 + data->map->width / 2;
		if (draw_end > data->map->width)
			draw_end = data->map->width;
		draw_vertical_line(&data->s_img, x, draw_start, draw_end, 0x00FFFF00);
		x++;
	}
	mlx_put_image_to_window(data->s_mlx.mlx, data->s_mlx.win, data->s_img.img,
		0, 0);
	return (1);
}
