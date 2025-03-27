/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:18:02 by llebugle          #+#    #+#             */
/*   Updated: 2025/03/28 00:37:04 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static void	init_hooks(t_data *data)
{
	mlx_hook(data->s_mlx.win, 2, 1L << 0, &key_pressed, data);
	mlx_hook(data->s_mlx.win, 3, 1L << 1, &key_released, data);
	mlx_hook(data->s_mlx.win, 6, 1L << 6, &mouse_handler, data);
	mlx_hook(data->s_mlx.win, 17, 0L, &leave, data);
	mlx_loop_hook(data->s_mlx.mlx, &draw_game, data);
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
	mlx_mouse_hide(data.s_mlx.mlx, data.s_mlx.win);
	init_hooks(&data);
	mlx_loop(data.s_mlx.mlx);
	return (0);
}
