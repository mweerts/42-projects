/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structures.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:05:30 by llebugle          #+#    #+#             */
/*   Updated: 2025/03/19 17:20:23 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTURES_H
# define STRUCTURES_H

# include "libft.h"

typedef enum e_map_element
{
	EMPTY = '0',
	OBSTACLE = '1',
	EAST = 'E',
	WEST = 'W',
	NORTH = 'N',
	SOUTH = 'S',
}					t_map_element;

typedef enum e_texture_wall
{
	TEX_NORTH,
	TEX_SOUTH,
	TEX_EAST,
	TEX_WEST,
	TEX_COUNT,
}					t_texture_wall;

typedef struct s_mlx
{
	void			*mlx;
	void			*win;
}					t_mlx;

typedef struct s_screen_coord
{
	int				x;
	int				y;
}					t_screen_coord;

typedef struct s_img
{
	void			*img;
	char			*addr;
	int				bits_per_pixel;
	int				line_length;
	int				endian;
	int				width;
	int				height;
}					t_img;

typedef struct s_texture
{
	t_img			img;
	int				*data;
	int				width;
	int				height;
	int				offset_x;
	int				offset_y;
}					t_texture;

typedef struct s_pos
{
	int				x;
	int				y;
}					t_pos;

typedef struct s_player
{
	double			pos_x;
	double			pos_y;
	double			dir_x;
	double			dir_y;
	double			plane_x;
	double			plane_y;
	int				mv_forward;
	int				mv_lateral;
	int				mv_rotate;
}					t_player;

typedef struct s_raycasting
{
	double			camera_x;
	double			ray_dir_x;
	double			ray_dir_y;
	int				ray_x;
	int				ray_y;
	int				hit;
	int				side;
	double			side_dist_x;
	double			side_dist_y;
	double			delta_dist_x;
	double			delta_dist_y;
	int				step_x;
	int				step_y;
	double			wall_distance;
}					t_raycasting;

typedef struct s_map
{
	int				width;
	int				height;
	int				floor_color;
	int				ceiling_color;
	char			**matrix;
	char			orientation_start;
	t_screen_coord	player_start;
}					t_map;

typedef struct s_data
{
	t_mlx			s_mlx;
	t_player		player;
	t_img			s_img;
	t_map			*map;
	t_texture		*tex[TEX_COUNT];
	int				max_row;
	int				max_col;
}					t_data;

#endif

