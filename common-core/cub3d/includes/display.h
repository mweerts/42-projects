/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:45:01 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/20 19:53:32 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DISPLAY_H
# define DISPLAY_H

// INIT

int	init_mlx(t_mlx *mlx);
int	init_hooks(t_data *data);

// DRAW

int	draw_pixel(t_img *img, int x, int y, int color);
int	draw_line(t_img *img, t_screen_coord p1, t_screen_coord p2, int color);

// EVENTS

int	leave(t_data *data);
int	key_pressed(int keycode, t_data *data);

#endif