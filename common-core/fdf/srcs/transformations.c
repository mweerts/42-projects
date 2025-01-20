/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transformations.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 16:49:16 by maxweert          #+#    #+#             */
/*   Updated: 2025/01/17 18:51:35 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	move(t_data *data, int keycode)
{
	if (keycode == K_AR_L)
		data->cam.x_offset -= 10;
	if (keycode == K_AR_R)
		data->cam.x_offset += 10;
	if (keycode == K_AR_U)
		data->cam.y_offset -= 10;
	if (keycode == K_AR_D)
		data->cam.y_offset += 10;
}

void	zoom(t_data *data, int keycode)
{
	if (keycode == K_NP_PLU)
	{
		if ((int)(data->cam.zoom * 1.1) == data->cam.zoom)
			data->cam.zoom++;
		else
			data->cam.zoom *= 1.1;
	}
	if (keycode == K_NP_MIN)
		data->cam.zoom *= 0.9;
}

void	rotate(t_data *data, int keycode)
{
	if (keycode == K_NP_1)
		data->cam.x_rot -= 0.05;
	if (keycode == K_NP_3)
		data->cam.x_rot += 0.05;
	if (keycode == K_NP_4)
		data->cam.y_rot -= 0.05;
	if (keycode == K_NP_6)
		data->cam.y_rot += 0.05;
	if (keycode == K_NP_7)
		data->cam.z_rot -= 0.05;
	if (keycode == K_NP_9)
		data->cam.z_rot += 0.05;
}

void	reset(t_data *data)
{
	data->cam.x_offset = 0;
	data->cam.y_offset = 0;
	data->cam.z_divisor = 2;
	data->cam.x_rot = 0;
	data->cam.y_rot = 0;
	data->cam.z_rot = 0;
	if (WIDTH / data->map.width / 2 < HEIGHT / data->map.height / 2)
		data->cam.zoom = WIDTH / data->map.width / 2;
	else
		data->cam.zoom = HEIGHT / data->map.height / 2;
}

void	flatten(t_data *data, int keycode)
{
	if (keycode == K_PG_UP)
		data->cam.z_divisor -= 0.1;
	if (keycode == K_PG_DOWN)
		data->cam.z_divisor += 0.1;
	if (data->cam.z_divisor < 0.1)
		data->cam.z_divisor = 0.1;
	if (data->cam.z_divisor > 15)
		data->cam.z_divisor = 15;
}
