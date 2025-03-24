/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 18:38:53 by llebugle          #+#    #+#             */
/*   Updated: 2025/03/17 18:38:54 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

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
			return (print_err(MSG_ERR_MALLOC), ERROR);
		i++;
	}
	matrix[i] = NULL;
	map->matrix = matrix;
	return (0);
}