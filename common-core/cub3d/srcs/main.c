/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:18:02 by llebugle          #+#    #+#             */
/*   Updated: 2025/03/26 14:40:28 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	main(int ac, char **av)
{
	t_data	data;

	init_data(&data);
	init_mlx(&data.s_mlx);
	if (parse_arguments(&data, ac, av))
		exit_with_error(NULL, &data);
	print_data(&data);
	init_player(&data.player, data.map->player_start,
		data.map->orientation_start);
	mlx_mouse_hide(data.s_mlx.mlx, data.s_mlx.win);
	init_hooks(&data);
	if (!init_img(&data.s_mlx, &data.s_img))
		return (EXIT_FAILURE);
	mlx_loop_hook(data.s_mlx.mlx, draw_game, &data);
	mlx_loop(data.s_mlx.mlx);
	return (0);
}
