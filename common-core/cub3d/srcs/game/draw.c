/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:43:07 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/25 00:21:55 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	draw_pixel(t_img *s_img, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x > WIDTH || y < 0 || y > HEIGHT)
		return (0);
	dst = s_img->addr + (y * s_img->line_length
			+ x * (s_img->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
	return (1);
}
