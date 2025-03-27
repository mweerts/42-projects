/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   portal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 17:04:19 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/27 03:17:47 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	*anim_routine(void *ptr)
{
	t_data *data;
	
	data = (t_data *)ptr;
	while (data->portal.stop == 0)
	{
		if (pthread_mutex_trylock(&data->portal.mutex) == 0)
		{
			//data->portal.curr_frame = (data->portal.curr_frame + 1) % NB_FRAMES;
			data->tex[TEX_PORTAL] = data->portal.frames[data->portal.curr_frame];
			pthread_mutex_unlock(&data->portal.mutex);
			ft_usleep(1000 / NB_FRAMES);
		}
	}
	pthread_mutex_unlock(&data->portal.stop_mutex);
	return (NULL);
}

static int	init_frames(t_data *data)
{
	data->portal.frames[0] = load_texture(data->s_mlx.mlx, "./assets/portal/portal_test.xpm");
	data->portal.frames[1] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_01.xpm");
	data->portal.frames[2] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_02.xpm");
	data->portal.frames[3] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_03.xpm");
	data->portal.frames[4] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_04.xpm");
	data->portal.frames[5] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_05.xpm");
	data->portal.frames[6] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_06.xpm");
	data->portal.frames[7] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_07.xpm");
	data->portal.frames[8] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_08.xpm");
	data->portal.frames[9] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_09.xpm");
	data->portal.frames[10] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_10.xpm");
	data->portal.frames[11] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_11.xpm");
	data->portal.frames[12] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_12.xpm");
	data->portal.frames[13] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_13.xpm");
	data->portal.frames[14] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_14.xpm");
	data->portal.frames[15] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_15.xpm");
	data->portal.frames[16] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_16.xpm");
	data->portal.frames[17] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_17.xpm");
	data->portal.frames[18] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_18.xpm");
	data->portal.frames[19] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_19.xpm");
	data->portal.frames[20] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_20.xpm");
	data->portal.frames[21] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_21.xpm");
	data->portal.frames[22] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_22.xpm");
	data->portal.frames[23] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_23.xpm");
	
	data->tex[TEX_PORTAL] = data->portal.frames[data->portal.curr_frame];
	return (1);
}

int	init_portal(t_data *data)
{
	data->portal.curr_frame = 0;
	data->portal.stop = 0;
	if (!init_frames(data))
		return (ft_putstr_fd(RED"error: failed to init frames.\n"RESET, 2), 0);
	if (pthread_mutex_init(&data->portal.mutex, NULL))
		return (ft_putstr_fd(RED"error: failed to create animation mutex.\n"RESET, 2), 0);
	if (pthread_mutex_init(&data->portal.stop_mutex, NULL))
		return (ft_putstr_fd(RED"error: failed to create stop mutex.\n"RESET, 2), 0);
	if (pthread_create(&data->portal.thread, NULL, anim_routine, data) != 0)
		return (ft_putstr_fd(RED"error: failed to create animation thread.\n"RESET, 2), 0);
	pthread_mutex_lock(&data->portal.stop_mutex);
	pthread_detach(data->portal.thread);
	return (1);
}
