/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 18:36:48 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/28 01:31:31 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	init_minimap(t_minimap *minimap)
{
	ft_memset(minimap, 0, sizeof(t_minimap));
	if (WIDTH / 16 > 0)
		minimap->radius = WIDTH / 16;
	minimap->center_x = WIDTH - minimap->radius - MINIMAP_OFFSET;
	minimap->center_y = HEIGHT - minimap->radius - MINIMAP_OFFSET;
	minimap->scale_factor = (float)minimap->radius / 8.0f;
	minimap->max_view_distance = (double)minimap->radius
		/ (double)minimap->scale_factor;
	minimap->ratio = (double)minimap->radius / minimap->max_view_distance;
}

int	init_img(t_mlx *s_mlx, t_img *s_img)
{
	s_img->img = mlx_new_image(s_mlx->mlx, WIDTH, HEIGHT);
	if (!s_img->img)
	{
		ft_putstr_fd("error: Image initialization failed.", 2);
		return (0);
	}
	s_img->addr = mlx_get_data_addr(s_img->img, &s_img->bits_per_pixel,
			&s_img->line_length, &s_img->endian);
	if (!s_img->addr)
	{
		ft_putstr_fd("error: Image initialization failed.", 2);
		return (0);
	}
	return (1);
}

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

void	init_ray(t_data *data, t_raycasting *ray, int x)
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
