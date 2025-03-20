/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 19:50:47 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/20 20:12:47 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	leave(t_data *data)
{
	// free_all
	(void)data;
	exit(0);
}

int	key_pressed(int keycode, t_data *data)
{
	if (keycode == K_ESC)
		leave(data);
	if (keycode == K_W)
		data->player.dir_y += 1;
	if (keycode == K_S)
		data->player.dir_y -= 1;
	if (keycode == K_D)
		data->player.dir_x += 1;
	if (keycode == K_A)
		data->player.dir_x -= 1;
	return (0);
}

int	key_released(int keycode, t_data *data)
{
	if (keycode == K_W)
		data->player.dir_y -= 1;
	if (keycode == K_S)
		data->player.dir_y += 1;
	if (keycode == K_D)
		data->player.dir_x -= 1;
	if (keycode == K_A)
		data->player.dir_x += 1;
	return (0);
}
