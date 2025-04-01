/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   portal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 17:05:29 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/29 20:18:24 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static t_map_element	get_portal_direction(t_raycasting *ray, int button)
{
	if (ray->side == 1)
	{
		if ((ray->ray_dir_y >= 0 && button == 1) || (ray->ray_dir_y < 0
				&& button == 3))
			return (SOUTH);
		else
			return (NORTH);
	}
	else
	{
		if ((ray->ray_dir_x >= 0 && button == 1) || (ray->ray_dir_x < 0
				&& button == 3))
			return (EAST);
		else
			return (WEST);
	}
}

static int	is_portal_pos_free(t_data *data, t_raycasting *ray,
		t_map_element dir)
{
	if (dir == NORTH && ray->ray_y + 1 < data->map->height
		&& data->map->matrix[ray->ray_y + 1][ray->ray_x] != 0)
		return (0);
	if (dir == SOUTH && ray->ray_y - 1 >= 0 && data->map->matrix[ray->ray_y
			- 1][ray->ray_x] != 0)
		return (0);
	if (dir == WEST && ray->ray_x + 1 < data->map->width
		&& data->map->matrix[ray->ray_y][ray->ray_x + 1] != 0)
		return (0);
	if (dir == EAST && ray->ray_x - 1 >= 0
		&& data->map->matrix[ray->ray_y][ray->ray_x - 1] != 0)
		return (0);
	return (1);
}

static int	is_destination_submap_ok(t_data *data, t_raycasting *ray,
		t_map_element dir, int **cpy)
{
	if (dir == NORTH && !flood_fill(data->map, cpy, ray->ray_y + 1, ray->ray_x))
		return (free_matrix(cpy, data->map->height), 1);
	else if (dir == SOUTH && !flood_fill(data->map, cpy, ray->ray_y - 1,
			ray->ray_x))
		return (free_matrix(cpy, data->map->height), 1);
	else if (dir == WEST && !flood_fill(data->map, cpy, ray->ray_y, ray->ray_x
			+ 1))
		return (free_matrix(cpy, data->map->height), 1);
	else if (dir == EAST && !flood_fill(data->map, cpy, ray->ray_y, ray->ray_x
			- 1))
		return (free_matrix(cpy, data->map->height), 1);
	return (free_matrix(cpy, data->map->height), 0);
}

static int	is_portal_possible(t_data *data, t_raycasting *ray, int button)
{
	int				**cpy;
	int				i;
	t_map_element	dir;

	dir = get_portal_direction(ray, button);
	if (data->map->matrix[ray->ray_y][ray->ray_x] != 1)
		return (0);
		
	if (!is_portal_pos_free(data, ray, dir))
		return (0);
	cpy = (int **)ft_calloc((data->map->height + 1), sizeof(int *));
	if (!cpy)
		return (print_err(MSG_ERR_MALLOC), -1);
	i = -1;
	while (++i < data->map->height)
	{
		cpy[i] = (int *)ft_calloc(data->map->width, sizeof(int));
		if (!cpy[i])
			return (free_matrix(cpy, i), print_err(MSG_ERR_MALLOC), -1);
		cpy[i] = ft_memcpy(cpy[i], data->map->matrix[i], data->map->width
				* sizeof(int));
	}
	return (is_destination_submap_ok(data, ray, dir, cpy));
}

void	create_portal(t_data *data, t_raycasting *ray, int button)
{
	if (!is_portal_possible(data, ray, button))
		return ;
	if (data->first_portal == 0)
	{
		data->portal1.x = ray->ray_x;
		data->portal1.y = ray->ray_y;
		data->portal1.old_tex = data->map->matrix[data->portal1.y]
		[data->portal1.x];
		data->portal1.orientation = get_portal_direction(ray, button);
	}
	else if (data->first_portal == 1)
	{
		data->portal2.x = ray->ray_x;
		data->portal2.y = ray->ray_y;
		data->portal2.old_tex = data->map->matrix[data->portal2.y]
		[data->portal2.x];
		data->portal2.orientation = get_portal_direction(ray, button);
	}
	data->first_portal ^= 1;
}
