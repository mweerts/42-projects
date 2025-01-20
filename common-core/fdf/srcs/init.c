/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 18:48:07 by maxweert          #+#    #+#             */
/*   Updated: 2025/01/17 19:12:13 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	init_hooks(t_data *data)
{
	mlx_hook(data->win, 2, 1L << 0, &key_pressed, data);
	mlx_hook(data->win, 17, 0L, &leave, data);
}

static t_img	init_img(void *mlx)
{
	t_img	img;

	img.img = mlx_new_image(mlx, WIDTH, HEIGHT);
	if (!img.img)
		ft_error("Error initializing new image.");
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel,
			&img.line_length, &img.endian);
	if (!img.addr)
		ft_error("Error initializing new image.");
	return (img);
}

static t_map	init_map(char *filename)
{
	t_map	map;
	int		i;

	map.height = get_map_height(filename);
	map.width = get_map_width(filename);
	map.colors_arr = malloc(map.height * sizeof(int *));
	map.z_arr = malloc(map.height * sizeof(int *));
	i = 0;
	while (i < map.height)
	{
		map.colors_arr[i] = ft_calloc(map.width, sizeof(int));
		map.z_arr[i] = ft_calloc(map.width, sizeof(int));
		i++;
	}
	return (map);
}

static t_cam	init_cam(t_map map)
{
	t_cam	cam;

	cam.projection = ISOMETRIC;
	cam.x_offset = 0;
	cam.y_offset = 0;
	cam.z_divisor = 2;
	cam.x_rot = 0;
	cam.y_rot = 0;
	cam.z_rot = 0;
	if ((WIDTH - MENU_WIDTH) / map.width / 2 < HEIGHT / map.height / 2)
		cam.zoom = (WIDTH - MENU_WIDTH) / map.width / 2;
	else
		cam.zoom = HEIGHT / map.height / 2;
	return (cam);
}

void	init_data(t_data *data, char *filename)
{
	data->mlx = mlx_init();
	if (!data->mlx)
		ft_error("Error with X11 server.");
	data->win = mlx_new_window(data->mlx, WIDTH, HEIGHT, "FdF");
	if (!data->win)
		ft_error("Error initializing window.");
	init_hooks(data);
	data->img = init_img(data->mlx);
	data->map = init_map(filename);
	data->cam = init_cam(data->map);
	parse_map(data, filename);
}
