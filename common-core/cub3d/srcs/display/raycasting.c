/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 15:01:55 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/29 17:12:08 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	dda(t_data *data, t_raycasting *ray)
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
		if (data->map->matrix[ray->ray_y][ray->ray_x] != 0)
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

	x = 0;
	compute_player_pos(data);
	pthread_mutex_lock(&data->anim.mutex);
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
	pthread_mutex_unlock(&data->anim.mutex);
	return (1);
}
