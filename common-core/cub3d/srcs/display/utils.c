/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 03:59:01 by llebugle          #+#    #+#             */
/*   Updated: 2025/03/31 03:59:03 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	blend_color(int background, int overlay, float alpha)
{
	t_rgb			bg;
	t_rgb			ov;
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;

	bg.r = (background >> 16) & 0xFF;
	bg.g = (background >> 8);
	bg.b = background & 0xFF;
	ov.r = (overlay >> 16) & 0xFF;
	ov.g = (overlay >> 8);
	ov.b = overlay & 0xFF;
	r = (unsigned char)(bg.r * (1 - alpha) + ov.r * alpha);
	g = (unsigned char)(bg.g * (1 - alpha) + ov.g * alpha);
	b = (unsigned char)(bg.b * (1 - alpha) + ov.b * alpha);
	return ((r << 16) | (g << 8) | b);
}