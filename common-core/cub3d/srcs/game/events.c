/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 19:50:47 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/29 19:25:54 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	leave(t_data *data)
{
	data->anim.stop = 1;
	pthread_mutex_lock(&data->anim.stop_mutex);
	pthread_mutex_unlock(&data->anim.stop_mutex);
	ft_usleep(50);
	clean_up(data);
	exit(0);
}

static void	toggle_mouse(t_data *data)
{
	data->mouse_off ^= 1;
	if (data->mouse_off)
		mlx_mouse_show(data->s_mlx.mlx, data->s_mlx.win);
	else
		mlx_mouse_hide(data->s_mlx.mlx, data->s_mlx.win);
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
	if (keycode == K_SHIFT)
		data->player.run = 2;
	if (keycode == K_M)
		toggle_mouse(data);
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
	if (keycode == K_SHIFT)
		data->player.run = 1;
	if (keycode == K_F)
		mouse_click(1, data);
	if (keycode == K_G)
		mouse_click(2, data);
	return (0);
}

int	mouse_handler(int x, int y, t_data *data)
{
	(void)y;
	if (data->mouse_off)
		return (0);
	if (x < WIDTH / 2)
		rotate_mouse(data, x);
	else if (x > WIDTH / 2)
		rotate_mouse(data, x);
	mlx_mouse_move(data->s_mlx.mlx, data->s_mlx.win, WIDTH / 2, HEIGHT / 2);
	return (0);
}
