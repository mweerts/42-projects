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

void	load_extra_textures(t_data *data)
{
	if (BONUS)
	{
		data->tex[TEX_NORTH] = load_texture(data->s_mlx.mlx, PINK_WALL);
		data->tex[TEX_SOUTH] = load_texture(data->s_mlx.mlx, PINK_WALL);
		data->tex[TEX_EAST] = load_texture(data->s_mlx.mlx, PINK_WALL);
		data->tex[TEX_WEST] = load_texture(data->s_mlx.mlx, PINK_WALL);
	}
	data->tex[TEX_FLOOR] = load_texture(data->s_mlx.mlx, FLOOR);
	data->tex[TEX_START] = load_texture(data->s_mlx.mlx, START);
	data->tex[TEX_INTERRUPTOR] = load_texture(data->s_mlx.mlx, INTERRUPTOR);
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
