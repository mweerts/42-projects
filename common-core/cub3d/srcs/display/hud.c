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

void	set_cross(t_data *data)
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

int	render_hud(t_data *data)
{
	set_cross(data);
	render_minimap(data);
	return (0);
}
