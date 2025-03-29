/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:45:01 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/29 19:15:11 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_H
# define GAME_H

// INIT

void	init_data(t_data *data);
int		init_player(t_player *player, t_coord pos, char start_position);
int		init_mlx(t_mlx *s_mlx);
int		init_win(t_mlx *s_mlx);

// EVENTS

int		leave(t_data *data);
int		key_pressed(int keycode, t_data *data);
int		key_released(int keycode, t_data *data);
int		mouse_handler(int x, int y, t_data *data);
int		mouse_click(int button, t_data *data);

// PLAYER

void	compute_player_pos(t_data *data);
void	rotate_mouse(t_data *data, int mouse_x);

// PORTAL

int		mouse_click(int button, t_data *data);
int		is_portal(t_data *data, int x, int y, int portal_i);

// TELEPORT

int 	should_tp(t_data *data, double step_x, double step_y);

#endif