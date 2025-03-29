/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   teleport.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 19:10:00 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/29 21:59:27 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static void	rotate_teleport(t_data *data, t_map_element entry_orientation,
		t_map_element exit_orientation)
{
	double	rot;
	double	old_dir_x;
	double	old_plane_x;

	if (entry_orientation == exit_orientation)
		rot = 3.1415;
	else if ((entry_orientation == NORTH && exit_orientation == EAST)
		|| (entry_orientation == EAST && exit_orientation == SOUTH)
		|| (entry_orientation == SOUTH && exit_orientation == WEST)
		|| (entry_orientation == WEST && exit_orientation == NORTH))
		rot = -3.1415 / 2;
	else if ((entry_orientation == NORTH && exit_orientation == WEST)
		|| (entry_orientation == WEST && exit_orientation == SOUTH)
		|| (entry_orientation == SOUTH && exit_orientation == EAST)
		|| (entry_orientation == EAST && exit_orientation == NORTH))
		rot = 3.1415 / 2;
	old_dir_x = data->player.dir_x;
	data->player.dir_x = data->player.dir_x * cos(rot) - data->player.dir_y
		* sin(rot);
	data->player.dir_y = old_dir_x * sin(rot) + data->player.dir_y * cos(rot);
	old_plane_x = data->player.plane_x;
	data->player.plane_x = data->player.plane_x * cos(rot)
		- data->player.plane_y * sin(rot);
	data->player.plane_y = old_plane_x * sin(rot) + data->player.plane_y
		* cos(rot);
}

static void	teleport(t_data *data, int portal_i,
		t_map_element entry_orientation)
{
	t_portal	*portal;

	if (portal_i == 1)
		portal = &data->portal2;
	else
		portal = &data->portal1;
	data->player.pos_x = portal->x + 0.5;
	data->player.pos_y = portal->y + 0.5;
	if (portal->orientation == WEST)
		data->player.pos_x = portal->x + 1.5;
	if (portal->orientation == EAST)
		data->player.pos_x = portal->x - 0.5;
	if (portal->orientation == NORTH)
		data->player.pos_y = portal->y + 1.5;
	if (portal->orientation == SOUTH)
		data->player.pos_y = portal->y - 0.5;
	rotate_teleport(data, entry_orientation, portal->orientation);
}

static int	check_portal1(t_data *data, double step_x, double step_y)
{
	int	new_cell_x;
	int	new_cell_y;

	new_cell_x = (int)(data->player.pos_x + step_x);
	new_cell_y = (int)(data->player.pos_y + step_y);
	if (is_portal(data, data->player.pos_x + step_x, data->player.pos_y
			+ step_y, 1))
	{
		if (new_cell_x < (int)data->player.pos_x
			&& data->portal1.orientation == WEST)
			return (teleport(data, 1, WEST), 1);
		if (new_cell_x > (int)data->player.pos_x
			&& data->portal1.orientation == EAST)
			return (teleport(data, 1, EAST), 1);
		if (new_cell_y < (int)data->player.pos_y
			&& data->portal1.orientation == NORTH)
			return (teleport(data, 1, NORTH), 1);
		if (new_cell_y > (int)data->player.pos_y
			&& data->portal1.orientation == SOUTH)
			return (teleport(data, 1, SOUTH), 1);
	}
	return (0);
}

static int	check_portal2(t_data *data, double step_x, double step_y)
{
	int	new_cell_x;
	int	new_cell_y;

	new_cell_x = (int)(data->player.pos_x + step_x);
	new_cell_y = (int)(data->player.pos_y + step_y);
	if (is_portal(data, data->player.pos_x + step_x, data->player.pos_y
			+ step_y, 2))
	{
		if (new_cell_x < (int)data->player.pos_x
			&& data->portal2.orientation == WEST)
			return (teleport(data, 2, WEST), 1);
		if (new_cell_x > (int)data->player.pos_x
			&& data->portal2.orientation == EAST)
			return (teleport(data, 2, EAST), 1);
		if (new_cell_y < (int)data->player.pos_y
			&& data->portal2.orientation == NORTH)
			return (teleport(data, 2, NORTH), 1);
		if (new_cell_y > (int)data->player.pos_y
			&& data->portal2.orientation == SOUTH)
			return (teleport(data, 2, SOUTH), 1);
	}
	return (0);
}

int	should_tp(t_data *data, double step_x, double step_y)
{
	if (data->portal1.x == -1 || data->portal2.x == -1)
		return (0);
	if (check_portal1(data, step_x, step_y))
		return (1);
	if (check_portal2(data, step_x, step_y))
		return (1);
	return (0);
}
