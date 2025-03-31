/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 18:44:08 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/29 17:11:52 by maxweert         ###   ########.fr       */
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

static void	cleanup_portal_frames(t_data *data)
{
	int	i;

	i = -1;
	while (++i < NB_FRAMES)
	{
		if (data->anim.frames[i])
		{
			if (data->anim.frames[i]->data)
				free(data->anim.frames[i]->data);
			if (data->anim.frames[i]->img.img)
				mlx_destroy_image(data->s_mlx.mlx,
					data->anim.frames[i]->img.img);
			free(data->anim.frames[i]);
			data->anim.frames[i] = NULL;
		}
	}
}

static void	cleanup_textures(t_data *data)
{
	int	i;

	i = -1;
	while (++i < TEX_COUNT)
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
	if (data->gun_tex && data->gun_tex->data)
		free(data->gun_tex->data);
	if (data->gun_tex && data->gun_tex->img.img)
		mlx_destroy_image(data->s_mlx.mlx, data->gun_tex->img.img);
	free(data->gun_tex);
	data->gun_tex = NULL;
}

void	clean_up(t_data *data)
{
	if (!data)
		return ;
	pthread_mutex_destroy(&data->anim.mutex);
	pthread_mutex_destroy(&data->anim.stop_mutex);
	cleanup_textures(data);
	cleanup_portal_frames(data);
	cleanup_map(data->map);
	if (data->s_mlx.win && data->s_img.img)
		mlx_destroy_image(data->s_mlx.mlx, data->s_img.img);
	if (data->s_mlx.win)
		mlx_destroy_window(data->s_mlx.mlx, data->s_mlx.win);
	//mlx_destroy_display(data->s_mlx.mlx);
	free(data->s_mlx.mlx);
}
