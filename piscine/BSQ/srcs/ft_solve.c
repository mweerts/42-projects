/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_solve.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 17:34:00 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/25 21:47:18 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_bsq.h"

int	**ft_create_matrix(t_map *map)
{
	int	**matrix;
	int	i;

	matrix = malloc(map->rows * sizeof(int *));
	if (!matrix)
		return (0);
	i = 0;
	while (i < map->rows)
	{
		matrix[i] = malloc(map->cols * sizeof(int));
		if (!matrix[i])
			return (0);
		i++;
	}
	return (matrix);
}

void	ft_free_matrix(int **matrix, t_map *map)
{
	int	i;

	if (matrix == NULL)
		return ;
	i = 0;
	while (i < map->rows)
	{
		if (matrix[i] != NULL)
			free(matrix[i]);
		i++;
	}
	free(matrix);
}

void	ft_initiate_matrix(int **matrix, t_map *map)
{
	int	i;
	int	j;

	i = 0;
	while (i < map->rows)
	{
		j = 0;
		while (j < map->cols)
		{
			if (i == 0 || j == 0)
			{
				if (map->map[i][j] == map->empty)
					matrix[i][j] = 1;
				else if (map->map[i][j] == map->obstacle)
					matrix[i][j] = 0;
			}
			else
				matrix[i][j] = 0;
			j++;
		}
		i++;
	}
}

t_square_coord	ft_compute_matrix(int **m, t_map *map)
{
	int				i;
	int				j;
	t_square_coord	coords;

	ft_init_coords(&coords);
	i = -1;
	while (++i < map->rows)
	{
		j = -1;
		while (++j < map->cols)
		{
			if ((i == 0 || j == 0) && m[i][j] > coords.max)
				ft_update_coords(m, i, j, &coords);
			else if ((i != 0 && j != 0) && map->map[i][j] == map->empty)
			{
				m[i][j] = ft_min(m[i - 1][j], m[i][j - 1], m[i - 1][j - 1]) + 1;
				if (m[i][j] > coords.max)
					ft_update_coords(m, i, j, &coords);
			}
			else if (i != 0 && j != 0)
				m[i][j] = 0;
		}
	}
	return (coords);
}

int	ft_solve(t_map *map)
{
	int				**matrix;
	int				i;
	int				j;
	t_square_coord	coords;

	i = 0;
	matrix = ft_create_matrix(map);
	if (!matrix)
		return (0);
	ft_initiate_matrix(matrix, map);
	coords = ft_compute_matrix(matrix, map);
	i = (coords.max_y - (coords.max - 1)) - 1;
	while (++i <= coords.max_y)
	{
		j = (coords.max_x - (coords.max - 1)) - 1;
		while (++j <= coords.max_x)
			map->map[i][j] = map->filled;
	}
	ft_free_matrix(matrix, map);
	ft_print_map(map);
	ft_free_map(map);
	return (1);
}
