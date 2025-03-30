/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:51:21 by llebugle          #+#    #+#             */
/*   Updated: 2025/03/28 19:09:49 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int is_on_edge(t_map *map, int x, int y)
{
	return (x == 0 || y  == 0 || x == map->width -1 || y == map->height -1);
}
int is_wall_on_edge(t_map *map, char c, int x, int y)
{
	// printf("pos = [%d,%d], char = %c\n",y, x, c);
	if ((c == '0' || c == ' ' || c == '\t') && is_on_edge(map, x, y))
		return (true);
	return (false);
}


static int	fill_matrix(t_map *map, int **matrix, char **line, int y)
{
	int	x;

	x = 0;
	matrix[y] = malloc(sizeof(int) * map->width);
	if (!matrix[y])
		return (print_err(MSG_ERR_MALLOC), ERROR);
	while (line[y][x])
	{	
		if (line[y][x] == ' ' || line[y][x] == '\t')
			matrix[y][x] = 0;
		else if (ft_is_charset((line[y][x]), "EWNS") == true)
		{
			if (is_on_edge(map, x, y) == true)
				return (print_err(MSG_PLAYER_ON_EDGE), ERROR);
			matrix[y][x] = 0;
		}
		else if (BONUS && is_wall_on_edge(map, line[y][x], x, y))
				matrix[y][x] = 2;
		else
			matrix[y][x] = line[y][x] - 48;
		x++;
	}
	while (x < map->width)
		matrix[y][x++] = 0;
	return (0);
}

int	create_matrix(t_map *map, char **line)
{
	int	i;
	int	**matrix;

	i = 0;
	matrix = (int **)malloc(sizeof(int *) * (map->height));
	if (!matrix)
		return (print_err(MSG_ERR_MALLOC), ERROR);
	while (i < map->height)
	{
		if (fill_matrix(map, matrix, line, i) == ERROR)
			return (ERROR);
		i++;
	}
	map->matrix = matrix;
	return (0);
}

int check_orientation(t_map *map, char c, t_coord pos)
{
	if (ft_is_charset(c, "EWNS") == true)
	{
		if (map->orientation_start)
			return (print_err(MSG_TOO_MANY_PLAYER), ERROR);
		map->orientation_start = c;
		map->player_start = pos;
	}
	return (0);
}

int	validate_map(t_map *map, char **line)
{
	int	i;
	int	j;
	char *valid_chars;
	
	i = -1;
	valid_chars = "01EWNS \t";
	if (BONUS)
		valid_chars = "01EWNSC \t";
	while (line[++i])
	{
		j = -1;
		while (line[i][++j])
		{
			check_orientation(map, line[i][j], (t_coord){j, i});
			if (ft_is_charset(line[i][j], valid_chars) == false)
				return (print_err(MSG_INVALID_CHAR), ERROR);
		}
		if (j > map->width)
			map->width = j;
	}
	if (!map->orientation_start)
		return (print_err(MSG_NO_PLAYER), ERROR);
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
	int	i;
	int	j;

	cpy = (int **)ft_calloc((map->height + 1), sizeof(int *));
	if (!cpy)
		return (print_err(MSG_ERR_MALLOC), ERROR);
	i = -1;
	while (++i < map->height)
	{
		cpy[i] = (int *)ft_calloc(map->width, sizeof(int));
		if (!cpy[i])
			return (free_matrix(cpy, i), print_err(MSG_ERR_MALLOC), ERROR);
		j = -1;
		while (++j < map->width)
			cpy[i][j] = map->matrix[i][j];
	}
	if (flood_fill(map, cpy, map->player_start.y, map->player_start.x) == ERROR)
		return (free_matrix(cpy, map->height), print_err(MSG_MAP_NOT_CLOSED),
			ERROR);
	free_matrix(cpy, map->height);
	return (0);
}
