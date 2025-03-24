/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:51:21 by llebugle          #+#    #+#             */
/*   Updated: 2025/03/21 17:51:23 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static int	get_map_size(t_map *map, char **line)
{
	int	i;
	int	j;

	i = 0;
	while (line[i])
	{
		j = 0;
		while (line[i][j])
		{
			if (ft_is_charset(line[i][j], "EWNS") == true)
			{
				if (map->orientation_start)
					return (print_err(MSG_TOO_MANY_PLAYER), ERROR);
				map->orientation_start = line[i][j];
				map->player_start = (t_coord){i, j};
			}
			if (ft_is_charset(line[i][j], "01EWNS \t") == false)
				return (print_err(MSG_INVALID_CHAR), ERROR);
			j++;
		}
		if (j > map->width)
			map->width = j;
		i++;
	}
	// if (!map->orientation_start)
	// 	return (print_err(MSG_NO_PLAYER), ERROR);
	return (map->height = i, 0);
}

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

int	is_map_closed(t_map *map)
{
	int	**cpy;
	int		i;
	int		j;

	cpy = (int **)ft_calloc((map->height + 1), sizeof(int *));
	if (!cpy)
		return (print_err(MSG_ERR_MALLOC), ERROR);
	i = 0;
	while (i < map->height)
	{
		j = 0;
		cpy[i] = (int *)ft_calloc(map->width, sizeof(int));
		if (!cpy[i])
			return (free_matrix(cpy, i), print_err(MSG_ERR_MALLOC), ERROR);
		while (j < map->width)
		{
			cpy[i][j] = map->matrix[i][j];
			j++;
		}
		i++;
	}
	if (flood_fill(map, cpy, map->player_start.x, map->player_start.y) == ERROR)
		return (free_matrix(cpy, map->height), print_err(MSG_MAP_NOT_CLOSED), ERROR);
	free_matrix(cpy, map->height);
	return (0);
}

int	parse_map(t_data *data, char **line)
{
	int	i;

	i = 0;
	if (!line || !line[0])
		return (ERROR);
	if (get_map_size(data->map, line))
		return (ERROR);
	if (create_matrix(data->map, line))
		return (ERROR);
	if (is_map_closed(data->map))
		return (ERROR);
	return (0);
}
