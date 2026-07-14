/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:30:25 by llebugle          #+#    #+#             */
/*   Updated: 2025/03/24 23:57:05 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static void	free_old_tex(t_data *data)
{
	int	i;

	i = -1;
	while (++i < 4)
	{
		if (data->tex[i])
		{
			if (data->tex[i]->data)
				free(data->tex[i]->data);
			if (data->tex[i]->img.img)
				mlx_destroy_image(data->s_mlx.mlx, data->tex[i]->img.img);
			free(data->tex[i]);
		}
		data->tex[i] = load_texture(data->s_mlx.mlx, PINK_WALL);
	}
}

void	load_extra_textures(t_data *data)
{
	if (BONUS)
		free_old_tex(data);
	data->tex[TEX_FLOOR] = load_texture(data->s_mlx.mlx, FLOOR);
	data->tex[TEX_SMALL_FRAME] = load_texture(data->s_mlx.mlx, SMALL_FRAME);
	data->tex[TEX_START] = load_texture(data->s_mlx.mlx, HUD_START);
	data->tex[TEX_CORE_DANGER] = load_texture(data->s_mlx.mlx, CORE_DANGER);
	data->tex[TEX_CORE] = load_texture(data->s_mlx.mlx, CORE);
	data->tex[TEX_EMPTY_FRAME] = load_texture(data->s_mlx.mlx, EMPTY_FRAME);
	data->tex[TEX_FRAME_DETAILS] = load_texture(data->s_mlx.mlx, FRAME_DETAILS);
	data->tex[TEX_MISSION_COMPLETE] = load_texture(data->s_mlx.mlx,
			MISSION_COMPLETE);
	data->tex[TEX_MISSION_FAILED] = load_texture(data->s_mlx.mlx,
			MISSION_FAILED);
}

t_texture	*load_texture(void *mlx, char *path)
{
	t_texture	*tex;
	int			tmp;

	if (!path)
		return (NULL);
	tex = malloc(sizeof(t_texture));
	if (!tex)
		return (NULL);
	ft_memset(tex, 0, (sizeof(t_texture)));
	tex->img.img = mlx_xpm_file_to_image(mlx, path, &tex->width, &tex->height);
	if (!tex->img.img)
		return (free(tex), NULL);
	tex->img.addr = mlx_get_data_addr(tex->img.img, &tex->img.bits_per_pixel,
			&tex->img.line_length, &tex->img.endian);
	tex->data = (int *)malloc(sizeof(int) * (tex->width * tex->height));
	if (!tex->data)
		return (mlx_destroy_image(mlx, tex->img.img), free(tex), NULL);
	tmp = -1;
	while (++tmp < tex->width * tex->height)
		tex->data[tmp] = ((int *)tex->img.addr)[tmp];
	return (tex);
}
