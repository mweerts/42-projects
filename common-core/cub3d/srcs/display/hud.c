/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 18:12:39 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/26 23:21:16 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

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
