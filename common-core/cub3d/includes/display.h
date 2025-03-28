/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 18:10:07 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/28 01:29:53 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DISPLAY_H
# define DISPLAY_H

// INIT

void	init_ray(t_data *data, t_raycasting *ray, int x);
int		init_img(t_mlx *s_mlx, t_img *s_img);

// DRAW

void	set_background(t_data *data);
int		draw_game(t_data *data);

// RAYCASTING

int		raycasting(t_data *data);
void	dda(t_data *data, t_raycasting *ray);

// TEXTURE

void	compute_tex(t_data *data, t_raycasting *ray, int x);

// HUD

void	set_cross(t_data *data);

#endif