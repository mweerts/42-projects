/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 17:04:19 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/29 17:05:27 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static int	check_frames_init(t_data *data)
{
	int	i;

	i = 0;
	while (i < NB_FRAMES)
	{
		if (data->anim.frames[i] == NULL)
			return (0);
		i++;
	}
	return (1);
}

int	init_portal_frames(t_data *data)
{
	t_texture	**frames;

	frames = data->anim.frames;
	frames[0] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_00.xpm");
	frames[1] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_01.xpm");
	frames[2] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_02.xpm");
	frames[3] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_03.xpm");
	frames[4] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_04.xpm");
	frames[5] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_05.xpm");
	frames[6] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_06.xpm");
	frames[7] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_07.xpm");
	frames[8] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_08.xpm");
	frames[9] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_09.xpm");
	if (!check_frames_init(data))
		return (ft_putstr_fd(RED"error: failed to init frames.\n"RESET, 2), 0);
	data->anim.curr_frame = frames[0];
	return (1);
}

void	*anim_routine(void *ptr)
{
	t_data	*data;

	data = (t_data *)ptr;
	while (data->anim.stop == 0)
	{
		if (pthread_mutex_trylock(&data->anim.mutex) == 0)
		{
			data->anim.frame_i = (data->anim.frame_i + 1) % NB_FRAMES;
			data->anim.curr_frame = data->anim.frames[data->anim.frame_i];
			pthread_mutex_unlock(&data->anim.mutex);
			ft_usleep(1000 / (NB_FRAMES * ANIM_SPEED));
		}
	}
	pthread_mutex_unlock(&data->anim.stop_mutex);
	return (NULL);
}

int	init_portal(t_data *data)
{
	data->anim.frame_i = 0;
	data->anim.stop = 0;
	if (pthread_mutex_init(&data->anim.mutex, NULL))
		return (ft_putstr_fd(RED"\
error: failed to create animation mutex.\n"RESET, 2), 0);
	if (pthread_mutex_init(&data->anim.stop_mutex, NULL))
		return (ft_putstr_fd(RED"error: failed to create stop mutex.\n"RESET,
				2), 0);
	if (pthread_create(&data->anim.thread, NULL, anim_routine, data) != 0)
		return (ft_putstr_fd(RED"\
error: failed to create animation thread.\n"RESET, 2), 0);
	pthread_mutex_lock(&data->anim.stop_mutex);
	pthread_detach(data->anim.thread);
	return (1);
}
