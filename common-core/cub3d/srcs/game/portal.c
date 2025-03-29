/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   portal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 17:05:29 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/29 19:38:49 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	is_portal(t_data *data, int x, int y, int portal_i)
{
	if (portal_i == 1 && x == data->portal1.x && y == data->portal1.y)
		return (1);
	if (portal_i == 2 && x == data->portal2.x && y == data->portal2.y)
		return (1);
	return (0);
}

static t_map_element	get_portal_direction(t_raycasting *ray, int button)
{
	if (ray->side == 1)
	{
		if ((ray->ray_dir_y >= 0 && button == 1) || (ray->ray_dir_y < 0
				&& button == 2))
			return (SOUTH);
		else
			return (NORTH);
	}
	else
	{
		if ((ray->ray_dir_x >= 0 && button == 1) || (ray->ray_dir_x < 0
				&& button == 2))
			return (EAST);
		else
			return (WEST);
	}
}

static int	is_portal_possible(t_data *data, t_raycasting *ray, int button)
{
	int				**cpy;
	int				i;
	int				j;
	t_map_element	dir;

	cpy = (int **)ft_calloc((data->map->height + 1), sizeof(int *));
	if (!cpy)
		return (print_err(MSG_ERR_MALLOC), -1);
	i = -1;
	while (++i < data->map->height)
	{
		cpy[i] = (int *)ft_calloc(data->map->width, sizeof(int));
		if (!cpy[i])
			return (free_matrix(cpy, i), print_err(MSG_ERR_MALLOC), -1);
		j = -1;
		while (++j < data->map->width)
			cpy[i][j] = data->map->matrix[i][j];
	}
	dir = get_portal_direction(ray, button);
	if (dir == NORTH && !flood_fill(data->map, cpy, ray->ray_y + 1, ray->ray_x))
		return (free_matrix(cpy, data->map->height), 1);
	else if (dir == SOUTH && !flood_fill(data->map, cpy, ray->ray_y - 1, ray->ray_x))
		return (free_matrix(cpy, data->map->height), 1);
	else if (dir == WEST && !flood_fill(data->map, cpy, ray->ray_y, ray->ray_x + 1))
		return (free_matrix(cpy, data->map->height), 1);
	else if (dir == EAST && !flood_fill(data->map, cpy, ray->ray_y, ray->ray_x - 1))
		return (free_matrix(cpy, data->map->height), 1);
	return (free_matrix(cpy, data->map->height), 0);
}

static void	create_portal(t_data *data, t_raycasting *ray, int button)
{
	if (data->first_portal == 0)
	{
		data->portal1.x = ray->ray_x;
		data->portal1.y = ray->ray_y;
		data->portal1.old_tex = data->map->matrix[data->portal1.y][data->portal1.x];
		data->portal1.orientation = get_portal_direction(ray, button);
	}
	else if (data->first_portal == 1)
	{
		data->portal2.x = ray->ray_x;
		data->portal2.y = ray->ray_y;
		data->portal2.old_tex = data->map->matrix[data->portal2.y][data->portal2.x];
		data->portal2.orientation = get_portal_direction(ray, button);
	}
	data->first_portal ^= 1;
}

int	mouse_click(int button, t_data *data)
{
	t_raycasting ray;

	init_ray(data, &ray, WIDTH / 2);
	dda(data, &ray);

	if (is_portal_possible(data, &ray, button))
		create_portal(data, &ray, button);
	printf("BUTTON : %d\n", button);
	return (1);
}