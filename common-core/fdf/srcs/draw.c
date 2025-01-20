/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:03:48 by maxweert          #+#    #+#             */
/*   Updated: 2025/01/18 04:01:08 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	img_pixel(t_data *data, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x > WIDTH || y < 0 || y > HEIGHT)
		return ;
	dst = data->img.addr + (y * data->img.line_length
			+ x * (data->img.bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

static void	set_background(t_data *data)
{
	int	x;
	int	y;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			if (x < MENU_WIDTH)
				img_pixel(data, x, y, MENU_COLOR);
			else
				img_pixel(data, x, y, BACKGROUND_COLOR);
			x++;
		}
		y++;
	}
}

void	draw_map(t_data *data)
{
	int	x;
	int	y;

	set_background(data);
	y = 0;
	while (y < data->map.height)
	{
		x = 0;
		while (x < data->map.width)
		{
			if (x < data->map.width - 1)
				img_line(data, project(data, x, y), project(data, x + 1, y));
			if (y < data->map.height - 1)
				img_line(data, project(data, x, y), project(data, x, y + 1));
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(data->mlx, data->win, data->img.img, 0, 0);
	draw_menu(data->mlx, data->win);
}

void	draw_menu(void *mlx, void *win)
{
	mlx_string_put(mlx, win, 5, 50, 0xFFFFFFFF, "Zoom: +/-");
	mlx_string_put(mlx, win, 5, 80, 0xFFFFFFFF, "Move: Arrows");
	mlx_string_put(mlx, win, 5, 110, 0xFFFFFFFF, "Flatten: PG UP/PG DOWN");
	mlx_string_put(mlx, win, 5, 140, 0xFFFFFFFF, "Rotate X: 1/3");
	mlx_string_put(mlx, win, 5, 170, 0xFFFFFFFF, "Rotate Y: 4/6");
	mlx_string_put(mlx, win, 5, 200, 0xFFFFFFFF, "Rotate Z: 7/9");
	mlx_string_put(mlx, win, 5, 230, 0xFFFFFFFF, "ISOMETRIC: I");
	mlx_string_put(mlx, win, 5, 260, 0xFFFFFFFF, "PARALLEL: P");
}
