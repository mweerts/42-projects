/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:18:02 by llebugle          #+#    #+#             */
/*   Updated: 2025/03/23 22:51:49 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

#define mapWidth 12
#define mapHeight 24

int worldMap[mapWidth][mapHeight]=
{
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

int main(int ac, char **av)
{
	t_data data;
	(void)ac;
	
	// data.map.ceiling_color = 0x000000FF;
	// data.map.floor_color = 0xFFFF0000;
    // for (int i = 0; i < mapWidth; i++)
    // {
    //     for (int j = 0; j < mapHeight; j++)
    //     {
    //         data.map.matrix[i][j] = worldMap[i][j];
    //     }
    // }
    data_init(&data);
	init_mlx(&data.s_mlx);
	if (parse_arguments(&data, ac, av))
		exit_with_error(NULL, &data);
	print_data(&data);
	init_player(&data.player, data.map->player_start);
	init_hooks(&data);
	init_img(&data.s_mlx, &data.s_img);
	mlx_loop_hook(data.s_mlx.mlx, raycasting, &data);
	mlx_loop(data.s_mlx.mlx);
	return (0);
}