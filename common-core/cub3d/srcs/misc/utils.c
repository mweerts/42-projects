/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 19:24:31 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/29 20:13:51 by maxweert         ###   ########.fr       */
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

int	is_portal(t_data *data, int x, int y, int portal_i)
{
	if (portal_i == 1 && x == data->portal1.x && y == data->portal1.y)
		return (1);
	if (portal_i == 2 && x == data->portal2.x && y == data->portal2.y)
		return (1);
	return (0);
}

unsigned char	*get_texture_pixel(t_texture *tex, int x, int y)
{
	return ((unsigned char *)&tex->img.addr[y * tex->img.line_length + x
			* (tex->img.bits_per_pixel / 8)]);
}
