/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memory.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 21:07:36 by apierret          #+#    #+#             */
/*   Updated: 2024/09/25 21:40:49 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_bsq.h"

int	ft_malloc_map(t_map **map, int rows)
{
	*map = (t_map *) malloc(sizeof(t_map));
	if (*map == NULL || rows <= 0)
		return (0);
	(*map)->rows = rows;
	(*map)->cols = 0;
	(*map)->map = (char **) malloc(sizeof(char *) * rows);
	if ((*map)->map == NULL)
		return (0);
	return (1);
}

int	ft_malloc_map_grid(t_map **map, int cols)
{
	int	i;

	i = 0;
	(*map)->cols = cols;
	while (i < (*map)->rows)
	{
		(*map)->map[i] = (char *) malloc(sizeof(char) * cols + 1);
		if ((*map)->map[i] == NULL)
			return (0);
		i++;
	}
	return (1);
}

void	ft_free_map(t_map *map)
{
	int	i;

	if (map == NULL)
		return ;
	if (map->map != NULL)
	{
		i = 0;
		while (i < map->rows)
		{
			if (map->map[i] != NULL)
			{
				free(map->map[i]);
			}
			i++;
		}
		free(map->map);
	}
	free(map);
}

char	*ft_realloc(char *old_buf, int new_size)
{
	int		i;
	char	*new_buf;

	new_buf = (char *)malloc(sizeof(char) * new_size);
	if (!new_buf)
		return (NULL);
	if (old_buf)
	{
		i = 0;
		while (old_buf[i] != '\0' && i < new_size - 1)
		{
			new_buf[i] = old_buf[i];
			i++;
		}
		new_buf[i] = '\0';
		free(old_buf);
	}
	else
		new_buf[0] = '\0';
	return (new_buf);
}
