/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:59:06 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/24 16:53:24 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static int	set_player_direction(t_player *player, char start_position)
{
	if (start_position == 'N')
	{
		player->dir_x = 0;
		player->dir_y = -1;
		player->plane_x = 0.66;
		player->plane_y = 0;		
	}
	if (start_position == 'S')
	{
		player->dir_x = 0;
		player->dir_y = 1;
		player->plane_x = -0.66;
		player->plane_y = 0;
	}
	if (start_position == 'W')
	{
		player->dir_x = -1;
		player->dir_y = 0;
		player->plane_x = 0;
		player->plane_y = -0.66;
	}
	if (start_position == 'E')
	{
		player->dir_x = 1;
		player->dir_y = 0;
		player->plane_x = 0;
		player->plane_y = 0.66;
	}
}

int	init_player(t_player *player, t_coord pos, char start_position)
{
	player->pos_x = pos.x;
	player->pos_y = pos.y;
	player->mv_forward = 0;
	player->mv_lateral = 0;
	player->mv_rotate = 0;
	set_player_direction(player, start_position);
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

int	init_img(t_mlx *s_mlx, t_img *s_img)
{
	s_img->img = mlx_new_image(s_mlx->mlx, WIDTH, HEIGHT);
	if (!s_img->img)
	{
		ft_putstr_fd("error: Image initialization failed.", 2);
		return (0);
	}
	s_img->addr = mlx_get_data_addr(s_img->img, &s_img->bits_per_pixel,
			&s_img->line_length, &s_img->endian);
	if (!s_img->addr)
	{
		ft_putstr_fd("error: Image initialization failed.", 2);
		return (0);
	}
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
