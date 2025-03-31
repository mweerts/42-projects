/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 18:12:39 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/29 19:44:56 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	draw_fps(t_data *data)
{
	char	*tmp;

	tmp = ft_itoa(data->s_fps.fps);
	if (tmp)
	{
		mlx_string_put(data->s_mlx.mlx, data->s_mlx.win, 50, 50, TEXT_COLOR,
			tmp);
		free(tmp);
		tmp = NULL;
	}
}

static void	set_cross(t_data *data)
{
	int	i;
	int	x;
	int	y;
	int	size;

	size = CROSS_SIZE;
	if (size % 2 == 0)
		size += 1;
	x = WIDTH / 2 - size / 2;
	y = HEIGHT / 2;
	i = 0;
	while (i < size)
	{
		draw_pixel(&data->s_img, x + i, y, POINTER_COLOR);
		i++;
	}
	x = WIDTH / 2;
	y = HEIGHT / 2 - size / 2;
	i = 0;
	while (i < size)
	{
		draw_pixel(&data->s_img, x, y + i, POINTER_COLOR);
		i++;
	}
}

static void	render_gun(t_data *data)
{
	int	x;
	int	y;
	int	color;
	int	offset_x;
	int	offset_y;

	offset_x = WIDTH / 10;
	offset_y = 200;
	y = -1;
	while (++y < data->gun_tex->height - offset_y)
	{
		x = -1;
		while (++x < data->gun_tex->width)
		{
			color = *(unsigned int *)get_texture_pixel(data->gun_tex, x, y);
			if (((color >> 24) & 0xFF) != 0xFF)
				draw_pixel(&data->s_img, WIDTH - data->gun_tex->width - offset_x
					+ x, HEIGHT - data->gun_tex->height + y + offset_y, color);
		}
	}
}

void	draw_interact(t_data *data)
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
		if (data->map->matrix[ray.ray_y][ray.ray_x] == 2)
			mlx_string_put(data->s_mlx.mlx, data->s_mlx.win, (WIDTH / 2) - 75,
				(HEIGHT / 2) - CROSS_SIZE, TEXT_COLOR,
				"Press [E] to shut down power.");
	}
}

void render_start(t_data *data)
{
    int x_pos = (WIDTH - data->tex[TEX_START]->width) / 2;
    int y_pos = (HEIGHT - data->tex[TEX_START]->height - 64) / 2;
    
    if (!data->started)
    {
        int y;
        int x;
        int color;
        
        for (y = 0; y < data->tex[TEX_START]->height && y < data->max_col; y++)
        {
            for (x = 0; x < data->tex[TEX_START]->width && x < data->max_col; x++)
            {
                color = *(unsigned int *)get_texture_pixel(data->tex[TEX_START], x, y);
                if (((color >> 24) & 0xFF) == 0xFF)
                {
                    draw_transparent_pixel(data, (t_coord){x_pos + x, y_pos + y}, color, 0.4);
                }
                else
                {
                    draw_pixel(&data->s_img, x_pos + x, y_pos + y, color);
                }
            }
        }
    }
}

int	render_hud(t_data *data)
{
	// set_cross(data);
	draw_mouse(data);
	render_minimap(data);
	render_start(data);
	if (data->started)
		render_gun(data);
	return (0);
}
