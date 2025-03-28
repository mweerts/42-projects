/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 19:24:31 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/28 01:29:46 by maxweert         ###   ########.fr       */
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
	data->s_fps.frames++;
	data->s_fps.delta_time = get_current_time() - data->s_fps.last_time;
	if (data->s_fps.delta_time >= 1000)
	{
		data->s_fps.fps = (int)(data->s_fps.frames / (data->s_fps.delta_time
					/ 1000));
		data->s_fps.last_time = get_current_time();
		data->s_fps.frames = 0;
	}
}

int	ft_usleep(size_t ms)
{
	size_t	start;

	start = get_current_time();
	while ((get_current_time() - start) < ms)
		usleep(500);
	return (0);
}
