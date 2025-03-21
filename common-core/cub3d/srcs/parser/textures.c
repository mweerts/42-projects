/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:30:25 by llebugle          #+#    #+#             */
/*   Updated: 2025/03/17 16:30:26 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"


// static int	store_map(int fd, t_data *data, char **map)
// {
// 	char	*line;

// 	while (1)
// 	{
// 		line = get_next_line(fd);
// 		if (!line)
// 			break ;
// 		*map = ft_strjoin_n_free(*map, line);
// 		free(line);
// 		if (!*map)
// 			return (set_err_msg(MALLOC_ERROR, data), -1);
// 		data->map->row++;
// 		if (data->map->row > (data->max_row / TILE_SIZE))
// 			return (set_err_msg(MAP_TOO_BIG, data), -1);
// 	}
// 	return (0);
// }

// int load_texture(t_data *data, char type, char *path)
// {
// 	data->textures[type] =
// }

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
	// tex->offset_x = (TILE_SIZE - tex->width) / 2;
	// tex->offset_y = TILE_SIZE - tex->height;
	// if (tex->height > TILE_SIZE)
	// 	tex->offset_y = -(tex->height - TILE_SIZE);
	tex->img.addr = mlx_get_data_addr(tex->img.img, &tex->img.bits_per_pixel,
			&tex->img.line_length, &tex->img.endian);
	tex->data = (int *)malloc(sizeof(int) * (tex->width * tex->height));
	if (!tex->data)
		return (mlx_destroy_image(mlx, tex->img.img), free(tex), NULL);
	tmp = -1;
	while (++tmp < tex->width * tex->height)
		tex->data[tmp] = ((int *)tex->img.addr)[tmp];
	return (mlx_destroy_image(mlx, tex->img.img), tex);
}
