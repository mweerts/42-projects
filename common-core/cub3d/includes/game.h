/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:45:01 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/23 21:21:38 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_H
# define GAME_H

// INIT

int		init_player(t_player *player);
int		init_mlx(t_mlx *s_mlx);
int		init_img(t_mlx *s_mlx, t_img *s_img);
int		init_hooks(t_data *data);

// DRAW

int		draw_pixel(t_img *s_img, int x, int y, int color);
int		draw_line(t_img *s_img, t_screen_coord p1, t_screen_coord p2, int color);
int		draw_vertical_line(t_img *s_img, int x, int start, int stop, int color);

// EVENTS

int		leave(t_data *data);
int		key_pressed(int keycode, t_data *data);
int		key_released(int keycode, t_data *data);

// BACKGROUND

int		set_background(t_data *data);

// RAYCASTING

int		raycasting(t_data *data);

// PLAYER

void	compute_player_pos(t_data *data);

#endif