/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:45:01 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/25 00:22:05 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_H
# define GAME_H

#include "structures.h"

// INIT

int		init_player(t_player *player, t_coord pos, char start_position);
int		init_mlx(t_mlx *s_mlx);
int		init_img(t_mlx *s_mlx, t_img *s_img);
int		init_hooks(t_data *data);

// DRAW

int		draw_pixel(t_img *s_img, int x, int y, int color);

// EVENTS

int		leave(t_data *data);
int		key_pressed(int keycode, t_data *data);
int		key_released(int keycode, t_data *data);
int		mouse_handler(int x, int y, t_data *data);

// BACKGROUND

int		set_background(t_data *data);

// RAYCASTING

int		raycasting(t_data *data);

// TEXTURE

void	compute_tex(t_data *data, t_raycasting *ray, int x);

// PLAYER

void	compute_player_pos(t_data *data);
void	rotate_mouse(t_data *data, double rotdir);

#endif