/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   teleport.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 19:10:00 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/29 20:07:53 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static void	teleport(t_data *data, int portal_i)
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
			return (printf("WEST\n"), teleport(data, 1), 1);
		if (new_cell_x > (int)data->player.pos_x
			&& data->portal1.orientation == EAST)
			return (printf("EAST\n"), teleport(data, 1), 1);
		if (new_cell_y < (int)data->player.pos_y
			&& data->portal1.orientation == NORTH)
			return (printf("NORTH\n"), teleport(data, 1), 1);
		if (new_cell_y > (int)data->player.pos_y
			&& data->portal1.orientation == SOUTH)
			return (printf("SOUTH\n"), teleport(data, 1), 1);
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
			return (printf("WEST\n"), teleport(data, 2), 1);
		if (new_cell_x > (int)data->player.pos_x
			&& data->portal2.orientation == EAST)
			return (printf("EAST\n"), teleport(data, 2), 1);
		if (new_cell_y < (int)data->player.pos_y
			&& data->portal2.orientation == NORTH)
			return (printf("NORTH\n"), teleport(data, 2), 1);
		if (new_cell_y > (int)data->player.pos_y
			&& data->portal2.orientation == SOUTH)
			return (printf("SOUTH\n"), teleport(data, 2), 1);
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
