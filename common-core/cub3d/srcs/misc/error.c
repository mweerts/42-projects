/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 16:21:13 by llebugle          #+#    #+#             */
/*   Updated: 2025/03/14 16:21:16 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	cleanup_textures(t_data *data)
{
	int	i;

	i = -1;
	if (!data->textures)
		return ;
	while (++i < TEX_COUNT)
	{
		if (data->textures[i])
		{
			if (data->textures[i]->data)
				free(data->textures[i]->data);
			free(data->textures[i]);
			data->textures[i] = NULL;
		}
	}
}

void	clean_up(t_data *data)
{
	if (!data)
		return ;
	if (data->win && data->img.img)
		mlx_destroy_window(data->mlx, data->win);
	if (data->win)
		mlx_destroy_image(data->mlx, data->img.img);
	cleanup_textures(data);
	mlx_destroy_display(data->mlx);
	free(data->mlx);
}

void	display_err_and_exit(const char *msg, t_data *data)
{
	ft_printf_fd(2, YELLOW);
	ft_printf_fd(2, "Error\n");
	if (msg)
		ft_printf_fd(2, "%s", msg, RESET);
	else if (errno)
		perror(strerror(errno));
	ft_printf_fd(2, RESET);
	clean_up(data);
	exit(EXIT_FAILURE);
}