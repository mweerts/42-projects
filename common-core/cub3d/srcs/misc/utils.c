/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 19:24:31 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/25 20:05:08 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	get_current_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (printf("Error: Error getting time of the day.\n"), -1);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	count_fps(t_data *data)
{
	char	*tmp;

	tmp = NULL;
	data->s_fps.frames++;
	data->s_fps.delta_time = get_current_time() - data->s_fps.last_time;
	if (data->s_fps.delta_time >= 1000)
	{
		data->s_fps.fps = (int)(data->s_fps.frames / (data->s_fps.delta_time
					/ 1000));
		data->s_fps.last_time = get_current_time();
		data->s_fps.frames = 0;
	}
	tmp = ft_itoa(data->s_fps.fps);
	if (tmp)
	{
		mlx_string_put(data->s_mlx.mlx, data->s_mlx.win, 50, 50, 0xFFFFFFFF, tmp);
		free(tmp);
		tmp = NULL;
	}
}
