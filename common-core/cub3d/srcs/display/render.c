/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:43:07 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/29 19:29:28 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	render_time_left(t_data *data, t_coord pos)
{
	char			time_str[16];
	long			remaining_ms;
	struct timeval	current_time;
	t_coord			p;
	long			elapsed_ms;

	if (!data->started)
		return ;
	gettimeofday(&current_time, NULL);
	elapsed_ms = ((current_time.tv_sec - data->start_time.tv_sec) * 1000)
		+ ((current_time.tv_usec - data->start_time.tv_usec) / 1000);
	remaining_ms = 0;
	if (data->time_left > elapsed_ms)
		remaining_ms = data->time_left - elapsed_ms;
	snprintf(time_str, sizeof(time_str), "%02ld:%02ld:%03ld", remaining_ms
		/ 60000, (remaining_ms / 1000) % 60, remaining_ms % 1000);
	p.x = data->minimap.center_x - 30;
	p.y = data->minimap.center_y - data->minimap.radius - 15;
	if (pos.x > 0 || pos.y > 0)
		p = pos;
	mlx_string_put(data->s_mlx.mlx, data->s_mlx.win, p.x, p.y, 0xFFFFFF,
		time_str);
	mlx_string_put(data->s_mlx.mlx, data->s_mlx.win, p.x, p.y + 1, 0xFFFFFF,
		time_str);
	data->remaining_ms = remaining_ms;
}

int	render_game(t_data *data)
{
	if (data->started && data->remaining_ms <= 0)
		data->game_end = LOOSE;
	if (BONUS)
		set_textured_background(data);
	else
		set_background(data);
	raycasting(data);
	count_fps(data);
	render_hud(data);
	mlx_put_image_to_window(data->s_mlx.mlx, data->s_mlx.win, data->s_img.img,
		0, 0);
	draw_fps(data);
	render_time_left(data, (t_coord){WIDTH - data->tex[TEX_SMALL_FRAME]->width
		- 28, ((double)(data->tex[TEX_SMALL_FRAME]->height) / 2) + 50 + 2});
	draw_interact(data);
	return (1);
}
