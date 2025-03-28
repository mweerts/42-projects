/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:43:07 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/28 02:33:12 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"


void	draw_minimap(t_data *data)
{
	int	size = WIDTH / 8;
	int	tile_size = size / 10;
	double	tmp_x;
	double	tmp_y;
	
	int	matrice[size][size];
	double theta = atan2(data->player.dir_x, data->player.dir_y);
	//printf("%f\n", data->player.dir_x);
	for (int y = 0; y < size; y++)
	{
		tmp_y = data->player.pos_y + ((double)y / (double)tile_size - (((double)size / 2) / (double)tile_size));
		for (int x = 0; x < size; x++)
		{
			tmp_x = data->player.pos_x + (x / (double)tile_size - (((double)size / 2) / (double)tile_size));
			if (tmp_x >= 0 && tmp_y >= 0 && tmp_x < data->map->width && tmp_y < data->map->height && data->map->matrix[(int)tmp_y][(int)tmp_x] == 0)
				draw_pixel(&data->minimap, x, y, 0xFF00FF00);
			else if (tmp_x >= 0 && tmp_y >= 0 && tmp_x < data->map->width && tmp_y < data->map->height && (data->map->matrix[(int)tmp_y][(int)tmp_x] == 2 || data->map->matrix[(int)tmp_y][(int)tmp_x] == -2))
				draw_pixel(&data->minimap, x, y, 0xFF0000FF);
			else
				draw_pixel(&data->minimap, x, y, 0xFFFF0000);
		}
	}
	
	double degree = 180.0 - (theta * (180.0 / 3.1415));
	//printf("%f\n", degree);
}

static void	draw_fps(t_data *data)
{
	char	*tmp;

	tmp = ft_itoa(data->s_fps.fps);
	if (tmp)
	{
		mlx_string_put(data->s_mlx.mlx, data->s_mlx.win, 50, 50, TEXT_COLOR,
			tmp);
		free(tmp);
		tmp = NULL;
	}
}

static void	draw_interact(t_data *data)
{
	int				curr_x;
	int				curr_y;
	t_raycasting	ray;

	curr_x = (int)data->player.pos_x;
	curr_y = (int)data->player.pos_y;
	init_ray(data, &ray, WIDTH / 2);
	dda(data, &ray);
	if (abs(curr_x - ray.ray_x) <= 1 && abs(curr_y - ray.ray_y) <= 1)
	{
		if (data->map->matrix[ray.ray_y][ray.ray_x] == 2)
			mlx_string_put(data->s_mlx.mlx, data->s_mlx.win, (WIDTH / 2) - 75,
				(HEIGHT / 2) - CROSS_SIZE, TEXT_COLOR,
				"Press [E] to open portal.");
		if (data->map->matrix[ray.ray_y][ray.ray_x] == -2)
			mlx_string_put(data->s_mlx.mlx, data->s_mlx.win, (WIDTH / 2) - 75,
				(HEIGHT / 2) - CROSS_SIZE, TEXT_COLOR,
				"Press [E] to close portal.");
	}
}

void	set_background(t_data *data)
{
	int	x;
	int	y;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			if (y > (HEIGHT / 2))
				draw_pixel(&data->s_img, x, y, data->map->floor_color);
			else
				draw_pixel(&data->s_img, x, y, data->map->ceiling_color);
			x++;
		}
		y++;
	}
}

int	draw_game(t_data *data)
{
	set_background(data);
	raycasting(data);
	count_fps(data);
	set_cross(data);
	draw_minimap(data);
	mlx_put_image_to_window(data->s_mlx.mlx, data->s_mlx.win, data->s_img.img,
		0, 0);
	mlx_put_image_to_window(data->s_mlx.mlx, data->s_mlx.win, data->minimap.img,
		WIDTH - (WIDTH / 8) - 20, 20);
	draw_fps(data);
	draw_interact(data);
	return (1);
}
