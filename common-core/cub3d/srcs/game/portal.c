/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   portal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 17:04:19 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/28 00:33:12 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static int	check_frames_init(t_data *data)
{
	int	i;

	i = 0;
	while (i < NB_FRAMES)
	{
		if (data->portal.frames[i] == NULL)
			return (0);
		i++;
	}
	return (1);
}

int	init_portal_frames(t_data *data)
{
	t_texture	**frames;

	frames = data->portal.frames;
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
	frames[10] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_10.xpm");
	frames[11] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_11.xpm");
	frames[12] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_12.xpm");
	frames[13] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_13.xpm");
	frames[14] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_14.xpm");
	frames[15] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_15.xpm");
	frames[16] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_16.xpm");
	frames[17] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_17.xpm");
	frames[18] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_18.xpm");
	frames[19] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_19.xpm");
	frames[20] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_20.xpm");
	frames[21] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_21.xpm");
	frames[22] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_22.xpm");
	frames[23] = load_texture(data->s_mlx.mlx, "./assets/portal/frame_23.xpm");
	if (!check_frames_init(data))
		return (ft_putstr_fd(RED"error: failed to init frames.\n"RESET, 2), 0);
	data->portal.curr_frame = frames[0];
	return (1);
}

void	*anim_routine(void *ptr)
{
	t_data	*data;

	data = (t_data *)ptr;
	while (data->portal.stop == 0)
	{
		if (pthread_mutex_trylock(&data->portal.mutex) == 0)
		{
			data->portal.frame_i = (data->portal.frame_i + 1) % NB_FRAMES;
			data->portal.curr_frame = data->portal.frames[data->portal.frame_i];
			pthread_mutex_unlock(&data->portal.mutex);
			ft_usleep(1000 / NB_FRAMES);
		}
	}
	pthread_mutex_unlock(&data->portal.stop_mutex);
	return (NULL);
}

int	init_portal(t_data *data)
{
	data->portal.frame_i = 0;
	data->portal.stop = 0;
	if (pthread_mutex_init(&data->portal.mutex, NULL))
		return (ft_putstr_fd(RED"\
error: failed to create animation mutex.\n"RESET, 2), 0);
	if (pthread_mutex_init(&data->portal.stop_mutex, NULL))
		return (ft_putstr_fd(RED"error: failed to create stop mutex.\n"RESET,
				2), 0);
	if (pthread_create(&data->portal.thread, NULL, anim_routine, data) != 0)
		return (ft_putstr_fd(RED"\
error: failed to create animation thread.\n"RESET, 2), 0);
	pthread_mutex_lock(&data->portal.stop_mutex);
	pthread_detach(data->portal.thread);
	return (1);
}

void	interact_portals(t_data *data)
{
	int				curr_x;
	int				curr_y;
	t_raycasting	ray;

	curr_x = (int)data->player.pos_x;
	curr_y = (int)data->player.pos_y;
	init_ray(data, &ray, WIDTH / 2);
	dda(data, &ray);
	if (abs(curr_x - ray.ray_x) <= 1 && abs(curr_y - ray.ray_y) <= 1)
	{
		if (data->map->matrix[ray.ray_y][ray.ray_x] == 2
			|| data->map->matrix[ray.ray_y][ray.ray_x] == -2)
			data->map->matrix[ray.ray_y][ray.ray_x] *= -1;
	}
}
