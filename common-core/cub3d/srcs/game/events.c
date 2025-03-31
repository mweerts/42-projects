/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 19:50:47 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/29 22:00:53 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	mouse_click(int button, int x, int y, t_data *data)
{
	t_raycasting	ray;

	(void)x;
	(void)y;
	init_ray(data, &ray, WIDTH / 2);
	dda(data, &ray);
	create_portal(data, &ray, button);
	return (1);
}

static void	toggle_mouse(t_data *data)
{
	data->mouse_off ^= 1;
	// if (data->mouse_off)
	// 	mlx_mouse_show(data->s_mlx.mlx, data->s_mlx.win);
	// else
	// 	mlx_mouse_hide(data->s_mlx.mlx, data->s_mlx.win);
}

int	key_pressed(int keycode, t_data *data)
{
	data->started = true;
	if (data->start_time.tv_usec <= 0)
	{
		data->time_left = TIME_LEFT_IN_MS;
		gettimeofday(&data->start_time, NULL);
	}
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
		data->player.run = 1.5;
	if (keycode == K_M)
		toggle_mouse(data);
	if (keycode == K_E)
		interact_interruptor(data);
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
	// mlx_mouse_move(data->s_mlx.mlx, data->s_mlx.win, WIDTH / 2, HEIGHT / 2);
	return (0);
}
