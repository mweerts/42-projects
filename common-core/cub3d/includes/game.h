/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:45:01 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/29 22:14:44 by maxweert         ###   ########.fr       */
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

int		key_pressed(int keycode, t_data *data);
int		key_released(int keycode, t_data *data);
int		mouse_handler(int x, int y, t_data *data);
int		mouse_click(int button, int x, int y, t_data *data);

// PLAYER

void	compute_player_pos(t_data *data);
void	rotate_mouse(t_data *data, int mouse_x, int old_x);

// PORTAL

void	create_portal(t_data *data, t_raycasting *ray, int button);

// TELEPORT

int		should_tp(t_data *data, double step_x, double step_y);

// ENDGAME

void	interact_interruptor(t_data *data);
void	render_mission_status(t_data *data, int status);

#endif