/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:18:02 by llebugle          #+#    #+#             */
/*   Updated: 2025/03/29 16:25:17 by maxweert         ###   ########.fr       */
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

static void	init_hooks(t_data *data)
{
	mlx_hook(data->s_mlx.win, 2, 1L << 0, &key_pressed, data);
	mlx_hook(data->s_mlx.win, 3, 1L << 1, &key_released, data);
	mlx_hook(data->s_mlx.win, 6, 1L << 6, &mouse_handler, data);
	mlx_hook(data->s_mlx.win, 17, 0L, &leave, data);
	mlx_loop_hook(data->s_mlx.mlx, &draw_game, data);
	mlx_mouse_hook(data->s_mlx.win, mouse_click, data);
}

int	main(int ac, char **av)
{
	t_data	data;

	init_data(&data);
	if (!init_mlx(&data.s_mlx))
		return (EXIT_FAILURE);
	if (parse_arguments(&data, ac, av))
		exit_with_error(NULL, &data);
	if (!init_portal_frames(&data))
		return (clean_up(&data), EXIT_FAILURE);
	print_data(&data);
	init_player(&data.player, data.map->player_start,
		data.map->orientation_start);
	if (!init_win(&data.s_mlx))
		return (clean_up(&data), EXIT_FAILURE);
	if (!init_img(&data.s_mlx, &data.s_img))
		return (clean_up(&data), EXIT_FAILURE);
	if (!init_portal(&data))
		return (clean_up(&data), EXIT_FAILURE);
	data.map->matrix[7][7] = 2;
	mlx_mouse_hide(data.s_mlx.mlx, data.s_mlx.win);
	mlx_mouse_move(data.s_mlx.mlx, data.s_mlx.win, WIDTH / 2, HEIGHT / 2);
	init_hooks(&data);
	mlx_loop(data.s_mlx.mlx);
	return (0);
}
