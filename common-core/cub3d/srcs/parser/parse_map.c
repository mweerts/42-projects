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
				map->player_start = (t_screen_coord){i, j};
			}
			if (ft_is_charset(line[i][j], "01EWNS \t") == false)
				return (print_err(MSG_INVALID_CHAR), ERROR);
			j++;
		}
		if (j > map->width)
			map->width = j;
		i++;
	}
	return (map->height = i, 0);
}

static int	fill_matrix(t_map *map, char **matrix, char **line)
{
	int	j;

	j = 0;
	
	*matrix = malloc(sizeof(char) * map->width + 1);
	if (!*matrix)
		return (ERROR);
	while ((*line)[j])
	{
		if ((*line)[j] == ' ' || (*line)[j] == '\t')
			(*matrix)[j] = '0';
		else
			(*matrix)[j] = (*line)[j];
		j++;
	}
	while (j < map->width)
		(*matrix)[j++] = '0';
	(*matrix)[j] = '\0';
	return (0);
}

int	create_matrix(t_map *map, char **line)
{
	int		i;
	char	**matrix;

	i = 0;
	matrix = (char **)malloc(sizeof(char *) * (map->height + 1));
	if (!matrix)
		return (print_err(MSG_ERR_MALLOC), ERROR);
	while (i < map->height)
	{
		if (fill_matrix(map, matrix + i, line + i) == ERROR)
			return (ft_free_tab(matrix), print_err(MSG_ERR_MALLOC), ERROR);
		i++;
	}
	matrix[i] = NULL;
	map->matrix = matrix;
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
	// if (!is_map_closed(data->map), line)
	// 	return (ERROR);
	return (0);
}

