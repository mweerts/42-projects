/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 18:12:39 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/29 19:44:56 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	draw_fps(t_data *data)
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

static void	set_cross(t_data *data)
{
	int	i;
	int	x;
	int	y;
	int	size;

	size = CROSS_SIZE;
	if (size % 2 == 0)
		size += 1;
	x = WIDTH / 2 - size / 2;
	y = HEIGHT / 2;
	i = 0;
	while (i < size)
	{
		draw_pixel(&data->s_img, x + i, y, CROSS_COLOR);
		i++;
	}
	x = WIDTH / 2;
	y = HEIGHT / 2 - size / 2;
	i = 0;
	while (i < size)
	{
		draw_pixel(&data->s_img, x, y + i, CROSS_COLOR);
		i++;
	}
}

static void	render_gun(t_data *data)
{
	int	x;
	int	y;
	int	color;
	int	offset_x;
	int	offset_y;

	offset_x = WIDTH / 10;
	offset_y = 200;
	y = -1;
	while (++y < data->gun_tex->height - offset_y)
	{
		x = -1;
		while (++x < data->gun_tex->width)
		{
			color = *(unsigned int *)get_texture_pixel(data->gun_tex, x, y);
			if (((color >> 24) & 0xFF) != 0xFF)
				draw_pixel(&data->s_img, WIDTH - data->gun_tex->width
				- offset_x + x, HEIGHT - data->gun_tex->height + y + offset_y, color);
		}
	}
}

int	render_hud(t_data *data)
{
	set_cross(data);
	// render_minimap(data);
	render_gun(data);
	return (0);
}
