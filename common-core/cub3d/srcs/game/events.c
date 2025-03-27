/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 19:50:47 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/27 17:22:51 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	leave(t_data *data)
{
	data->portal.stop = 1;
	pthread_mutex_lock(&data->portal.stop_mutex);
	pthread_mutex_unlock(&data->portal.stop_mutex);
	ft_usleep(50);
	clean_up(data);
	exit(0);
}

int	key_pressed(int keycode, t_data *data)
{
	if (keycode == K_ESC)
		leave(data);
	if (keycode == K_W)
		data->player.mv_forward += 1;
	if (keycode == K_S)
		data->player.mv_forward += -1;
	if (keycode == K_A)
		data->player.mv_lateral += -1;
	if (keycode == K_D)
		data->player.mv_lateral += 1;
	if (keycode == K_AR_L)
		data->player.mv_rotate += -1;
	if (keycode == K_AR_R)
		data->player.mv_rotate += 1;
	return (0);
}

int	key_released(int keycode, t_data *data)
{
	if (keycode == K_W)
		data->player.mv_forward += -1;
	if (keycode == K_S)
		data->player.mv_forward += 1;
	if (keycode == K_A)
		data->player.mv_lateral += 1;
	if (keycode == K_D)
		data->player.mv_lateral += -1;
	if (keycode == K_AR_L)
		data->player.mv_rotate += 1;
	if (keycode == K_AR_R)
		data->player.mv_rotate += -1;
	return (0);
}

int	mouse_handler(int x, int y, t_data *data)
{
	(void)y;
	if (x < WIDTH / 2)
		rotate_mouse(data, -1, x);
	else if (x > WIDTH / 2)
		rotate_mouse(data, 1, x);
	mlx_mouse_move(data->s_mlx.mlx, data->s_mlx.win, WIDTH / 2, HEIGHT / 2);
	return (0);
}
