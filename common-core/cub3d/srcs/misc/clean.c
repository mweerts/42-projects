/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 18:44:08 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/27 00:31:35 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	free_matrix(int **matrix, int i)
{
	if (!matrix || !*matrix)
		return ;
	if (!i)
	{
		free(matrix);
		return ;
	}
	while (i--)
		if (matrix[i])
			free(matrix[i]);
	free(matrix);
}

static void	cleanup_map(t_map *map)
{
	free_matrix(map->matrix, map->height);
	free(map);
}

static void	cleanup_portal(t_data *data)
{
	int	i;

	i = -1;
	while (++i < NB_FRAMES)
	{
		if (data->portal.frames[i])
		{
			if (data->portal.frames[i]->data)
				free(data->portal.frames[i]->data);
			if (data->portal.frames[i]->img.img)
				mlx_destroy_image(data->s_mlx.mlx,
					data->portal.frames[i]->img.img);
			free(data->portal.frames[i]);
			data->portal.frames[i] = NULL;
		}
	}
}

static void	cleanup_textures(t_data *data)
{
	int	i;

	i = -1;
	while (++i < TEX_COUNT - 1)
	{
		if (data->tex[i])
		{
			if (data->tex[i]->data)
				free(data->tex[i]->data);
			if (data->tex[i]->img.img)
				mlx_destroy_image(data->s_mlx.mlx, data->tex[i]->img.img);
			free(data->tex[i]);
			data->tex[i] = NULL;
		}
	}
}

void	clean_up(t_data *data)
{
	if (!data)
		return ;
	pthread_mutex_destroy(&data->portal.mutex);
	pthread_mutex_destroy(&data->portal.stop_mutex);
	cleanup_textures(data);
	cleanup_portal(data);
	cleanup_map(data->map);
	if (data->s_mlx.win && data->s_img.img)
		mlx_destroy_image(data->s_mlx.mlx, data->s_img.img);
	if (data->s_mlx.win)
		mlx_destroy_window(data->s_mlx.mlx, data->s_mlx.win);
	mlx_destroy_display(data->s_mlx.mlx);
	free(data->s_mlx.mlx);
}
