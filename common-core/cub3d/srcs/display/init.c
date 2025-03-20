/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:59:06 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/20 19:52:54 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	init_hooks(t_data *data)
{
	mlx_hook(data->mlx.win, 2, 1L << 0, &key_pressed, data);
	mlx_hook(data->mlx.win, 17, 0L, &leave, data);
	return (1);
}

static int	init_img(t_mlx *mlx)
{
	mlx->img.img = mlx_new_image(mlx->mlx, WIDTH, HEIGHT);
	if (!mlx->img.img)
	{
		ft_putstr_fd("error: Image initialization failed.", 2);
		return (0);
	}
	mlx->img.addr = mlx_get_data_addr(mlx->img.img, &mlx->img.bits_per_pixel,
			&mlx->img.line_length, &mlx->img.endian);
	if (!mlx->img.addr)
	{
		ft_putstr_fd("error: Image initialization failed.", 2);
		return (0);
	}
	return (1);
}

int	init_mlx(t_mlx *mlx)
{
	mlx->mlx = mlx_init();
	if (!mlx->mlx)
	{
		ft_putstr_fd("error: MLX initialization failed.", 2);
		return (0);
	}
	mlx->win = mlx_new_window(mlx->mlx, WIDTH, HEIGHT, "cub3D");
	if (!mlx->win)
	{
		ft_putstr_fd("error: Window initialization failed.", 2);
		return (0);
	}
	if (!init_img(mlx))
		return (0);
	return (1);
}
