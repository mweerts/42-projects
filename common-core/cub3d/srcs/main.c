/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:18:02 by llebugle          #+#    #+#             */
/*   Updated: 2025/03/23 20:15:41 by maxweert         ###   ########.fr       */
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
	(void)av;
	data.map.ceiling_color = 0x000000FF;
	data.map.floor_color = 0xFFFF0000;
	data.player.pos_x = 5;
	data.player.pos_y = 5;
	data.player.mv_forward = 0;
	data.player.mv_lateral = 0;
	data.player.mv_rotate = 0;
	data.player.dir_x = 0;
	data.player.dir_y = -1;
	data.player.plane_x = 0.66;
	data.player.plane_y = 0;
    for (int i = 0; i < mapWidth; i++)
    {
        for (int j = 0; j < mapHeight; j++)
        {
            data.map.matrix[i][j] = worldMap[i][j];
        }
    }
	init_mlx(&data.s_mlx);
	init_hooks(&data);
	init_img(&data.s_mlx, &data.s_img);
	mlx_loop_hook(data.s_mlx.mlx, raycasting, &data);
	mlx_loop(data.s_mlx.mlx);
	return (0);
}