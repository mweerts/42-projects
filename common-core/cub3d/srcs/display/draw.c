/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:43:07 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/25 20:34:15 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	draw_game(t_data *data)
{
	char	*tmp;

	tmp = NULL;
	set_background(data);
	raycasting(data);
	count_fps(data);
	set_cross(data);
	mlx_put_image_to_window(data->s_mlx.mlx, data->s_mlx.win, data->s_img.img,
		0, 0);
	tmp = ft_itoa(data->s_fps.fps);
	if (tmp)
	{
		mlx_string_put(data->s_mlx.mlx, data->s_mlx.win, 50, 50, 0xFFFFFFFF,
			tmp);
		free(tmp);
		tmp = NULL;
	}
	return (1);
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

void	draw_pixel(t_img *s_img, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x > WIDTH || y < 0 || y > HEIGHT)
		return ;
	dst = s_img->addr + (y * s_img->line_length
			+ x * (s_img->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}
