/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:59:06 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/23 20:36:28 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	init_hooks(t_data *data)
{
	mlx_hook(data->s_mlx.win, 2, 1L << 0, &key_pressed, data);
	mlx_hook(data->s_mlx.win, 3, 1L << 1, &key_released, data);
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
