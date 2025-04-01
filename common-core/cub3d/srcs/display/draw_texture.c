/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_texture.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 00:46:57 by llebugle          #+#    #+#             */
/*   Updated: 2025/04/01 00:47:03 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	draw_texture(t_data *data, t_texture *tex, t_coord pos, float alpha)
{
	int	x;
	int	y;
	int	color;

	y = -1;
	while (++y < tex->height)
	{
		x = -1;
		while (++x < tex->width)
		{
			color = *(unsigned int *)get_texture_pixel(tex, x, y);
			if (((color >> 24) & 0xFF) == 0xFF)
			{
				if (alpha <= 1)
					draw_transparent_pixel(data, (t_coord){pos.x + x, pos.y
						+ y}, color, alpha);
			}
			else
				draw_pixel(&data->s_img, (int)(pos.x + x), (int)(pos.y + y),
					color);
		}
	}
}

/* 
** Function to draw a texture centered at given coordinates
** Useful for UI elements like crosshairs or reticles
*/
void	draw_texture_centered(t_data *data, t_texture *tex, t_coord center, float alpha)
{
	t_coord	pos;
	
	pos.x = center.x - (int)(tex->width / 2);
	pos.y = center.y - (int)(tex->height / 2);
	draw_texture(data, tex, pos, alpha);
}