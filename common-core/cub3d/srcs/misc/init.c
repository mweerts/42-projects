/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:27:18 by llebugle          #+#    #+#             */
/*   Updated: 2025/03/21 18:27:19 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void data_init(t_data *data)
{
	ft_memset(data, 0, sizeof(t_data));
	data->mlx = mlx_init();
	if (!data->mlx)
		exit_with_error(NULL, data);
	errno = 0;
	data->map = malloc(sizeof(t_map));
	if (!data->map)
		exit_with_error(NULL, data);
	ft_memset(data->map, 0, sizeof(t_map));
	data->map->matrix = NULL;
	data->map->ceiling_color = -1;
	data->map->floor_color = -1;
}
