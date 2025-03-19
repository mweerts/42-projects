/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:45:01 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/19 16:38:31 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DISPLAY_H
# define DISPLAY_H

// INIT

int	init_mlx(t_mlx *mlx);

// DRAW

int	draw_pixel(t_img *img, int x, int y, int color);
int	draw_line(t_img *img, t_coord p1, t_coord p2, int color);

#endif