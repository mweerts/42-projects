/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:45:01 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/28 00:33:24 by maxweert         ###   ########.fr       */
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

// PLAYER

void	compute_player_pos(t_data *data);
void	rotate_mouse(t_data *data, int mouse_x);

// PORTAL

int		init_portal_frames(t_data *data);
int		init_portal(t_data *data);
void	interact_portals(t_data *data);

// TELEPORT

void	teleport(t_data *data, double old_x, double old_y);

#endif