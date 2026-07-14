/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 18:09:35 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/25 19:01:43 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_bsq.h"

void	ft_print_map(t_map *map)
{
	int	i;

	i = 0;
	while (i < map->rows)
	{
		write(1, map->map[i], map->cols);
		write(1, "\n", 1);
		i++;
	}
}

int	ft_validate_char(char c, t_map *map)
{
	return (c == map->empty || c == map->obstacle || c == map->filled);
}

int	ft_different_char(t_map *map)
{
	return ((map->empty != map->obstacle)
		&& (map->empty != map->filled) && (map->obstacle != map->filled));
}
