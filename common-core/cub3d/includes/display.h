/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 18:10:07 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/29 22:10:03 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DISPLAY_H
# define DISPLAY_H

# define MINIMAP_OFFSET 20

// INIT

void	init_ray(t_data *data, t_raycasting *ray, int x);
int		init_img(t_mlx *s_mlx, t_img *s_img);

// DRAW

void	draw_pixel(t_img *s_img, int x, int y, int color);
void	set_background(t_data *data);
int		render_game(t_data *data);
void	draw_transparent_pixel(t_data *data, t_coord coord, int color,
			float alpha);
void	draw_mouse(t_data *data);
void	draw_texture(t_data *data, t_texture *tex, t_coord pos, float alpha);
void	draw_texture_centered(t_data *data, t_texture *tex, t_coord center,
			float alpha);
void	draw_circle(t_data *data, t_coord center, int radius, int color);

// RAYCASTING

int		raycasting(t_data *data);
void	dda(t_data *data, t_raycasting *ray);

// TEXTURE

void	compute_tex(t_data *data, t_raycasting *ray, int x);

// HUD

void	draw_fps(t_data *data);
int		render_hud(t_data *data);
void	draw_interact(t_data *data);

// BACKGROUND

void	set_textured_background(t_data *data);
int		init_portal(t_data *data);

// MINIMAP

void	render_minimap(t_data *data);
void	init_minimap(t_minimap *minimap);
void	draw_line(t_data *data, t_coord start, t_coord end, int color);
int		render_player_in_minimap(t_data *data, t_minimap *minimap);
void	show_map(t_data *data);

// ANIMATION

int		init_portal_frames(t_data *data);
int		init_portal(t_data *data);
void	render_time_left(t_data *data, t_coord pos);
int		blend_color(int background, int overlay, float alpha);

// UTILS
void	calculate_triangle_points(t_coord center, t_triangle *t, int size);
void	calculate_direction(t_data *data, t_triangle *t, double *dir_length);
void	toggle_mouse(t_data *data);

// FULLMAP
void	draw_map_cells(t_data *data, t_fullmap *map);

#endif
