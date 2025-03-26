/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:59:06 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/26 15:34:50 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	init_data(t_data *data)
{
	data->s_mlx.mlx = NULL;
	data->s_mlx.win = NULL;
	ft_memset(data, 0, sizeof(t_data));
	errno = 0;
	data->map = malloc(sizeof(t_map));
	if (!data->map)
		exit_with_error(NULL, data);
	ft_memset(data->map, 0, sizeof(t_map));
	data->map->matrix = NULL;
	data->map->ceiling_color = -1;
	data->map->floor_color = -1;
	data->s_fps.delta_time = 0;
	data->s_fps.frames = 0;
	data->s_fps.last_time = get_current_time();
}

static void	init_fov(t_player *player, char start_position)
{
	if (start_position == 'N')
	{
		player->dir_y = -1;
		player->plane_x = 1 * FOV_RATIO;
	}
	if (start_position == 'S')
	{
		player->dir_y = 1;
		player->plane_x = -1 * FOV_RATIO;
	}
	if (start_position == 'W')
	{
		player->dir_x = -1;
		player->plane_y = -1 * FOV_RATIO;
	}
	if (start_position == 'E')
	{
		player->dir_x = 1;
		player->plane_y = 1 * FOV_RATIO;
	}
}

int	init_player(t_player *player, t_coord pos, char start_position)
{
	player->pos_x = pos.x + 0.5;
	player->pos_y = pos.y + 0.5;
	player->mv_forward = 0;
	player->mv_lateral = 0;
	player->mv_rotate = 0;
	player->dir_x = 0;
	player->dir_y = 0;
	player->plane_x = 0;
	player->plane_y = 0;
	init_fov(player, start_position);
	return (1);
}

int	init_hooks(t_data *data)
{
	mlx_hook(data->s_mlx.win, 2, 1L << 0, &key_pressed, data);
	mlx_hook(data->s_mlx.win, 3, 1L << 1, &key_released, data);
	mlx_hook(data->s_mlx.win, 6, 1L << 6, &mouse_handler, data);
	mlx_hook(data->s_mlx.win, 17, 0L, &leave, data);
	return (1);
}

int	init_mlx(t_mlx *s_mlx)
{
	s_mlx->mlx = mlx_init();
	if (!s_mlx->mlx)
	{
		ft_putstr_fd("error: MLX initialization failed.", 2);
		return (0);
	}
	s_mlx->win = mlx_new_window(s_mlx->mlx, WIDTH, HEIGHT, "cub3D");
	if (!s_mlx->win)
	{
		ft_putstr_fd("error: Window initialization failed.", 2);
		return (0);
	}
	return (1);
}
