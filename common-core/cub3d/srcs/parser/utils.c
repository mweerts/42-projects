/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 15:57:23 by maxweert          #+#    #+#             */
/*   Updated: 2025/04/02 15:57:25 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	flood_fill(t_map *map, int **matrix, int x, int y)
{
	int	result;

	if (x < 0 || y < 0 || x >= map->height || y >= map->width)
		return (ERROR);
	if (matrix[x][y] == OBSTACLE || matrix[x][y] == VISITED)
		return (SUCCESS);
	matrix[x][y] = VISITED;
	result = SUCCESS;
	if (flood_fill(map, matrix, x + 1, y) == ERROR)
		result = ERROR;
	if (flood_fill(map, matrix, x, y + 1) == ERROR)
		result = ERROR;
	if (flood_fill(map, matrix, x - 1, y) == ERROR)
		result = ERROR;
	if (flood_fill(map, matrix, x, y - 1) == ERROR)
		result = ERROR;
	return (result);
}

static inline int	is_on_edge(t_map *map, int x, int y)
{
	return (x == 0 || y == 0 || x == map->width - 1 || y == map->height - 1);
}

int	check_orientation(t_map *map, char c, t_coord pos)
{
	if (ft_is_charset(c, "EWNS") == true)
	{
		if (map->orientation_start)
			return (print_err(MSG_TOO_MANY_PLAYER), ERROR);
		if (is_on_edge(map, (int)pos.x, (int)pos.y) == true)
			return (print_err(MSG_PLAYER_ON_EDGE), ERROR);
		map->orientation_start = c;
		map->player_start = pos;
	}
	return (0);
}
