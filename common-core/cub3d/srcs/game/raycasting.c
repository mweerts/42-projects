/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 15:01:55 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/25 01:59:26 by maxweert         ###   ########.fr       */
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
	ray->camera_x = 2 * x / (double)WIDTH - 1;
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
		ray->wall.wall_distance = (ray->ray_x - data->player.pos_x + (1
					- ray->step_x) / 2) / ray->ray_dir_x;
	else
		ray->wall.wall_distance = (ray->ray_y - data->player.pos_y + (1
					- ray->step_y) / 2) / ray->ray_dir_y;
	if (ray->wall.wall_distance < 0.001)
		ray->wall.wall_distance = 0.001;
}

int	raycasting(t_data *data)
{
	t_raycasting	ray;
	int				x;

	set_background(data);
	compute_player_pos(data);
	x = 0;
	while (x < WIDTH)
	{
		init_ray(data, &ray, x);
		dda(data, &ray);
		ray.wall.line_height = (int)(HEIGHT / ray.wall.wall_distance);
		ray.wall.draw_start = -ray.wall.line_height / 2 + HEIGHT / 2;
		if (ray.wall.draw_start < 0)
			ray.wall.draw_start = 0;
		ray.wall.draw_end = ray.wall.line_height / 2 + HEIGHT / 2;
		if (ray.wall.draw_end > HEIGHT)
			ray.wall.draw_end = HEIGHT;
		compute_tex(data, &ray, x);
		x++;
	}
	mlx_put_image_to_window(data->s_mlx.mlx, data->s_mlx.win, data->s_img.img,
		0, 0);
	return (1);
}
