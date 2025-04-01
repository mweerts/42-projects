/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structures.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:05:30 by llebugle          #+#    #+#             */
/*   Updated: 2025/03/29 22:09:55 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTURES_H
# define STRUCTURES_H

# include "libft.h"
# include <pthread.h>
# include <sys/time.h>

# define NB_FRAMES 10

typedef enum e_map_element
{
	EMPTY = '0',
	PORTAL = 'P',
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
	TEX_FLOOR,
	TEX_INTERRUPTOR,
	TEX_START,
	TEX_SMALL_FRAME,
	TEX_EMPTY_FRAME,
	TEX_MISSION_FAILED,
	TEX_FRAME_DETAILS,
	TEX_COUNT,
}					t_texture_wall;

typedef struct s_coord
{
	double			x;
	double			y;
}					t_coord;

typedef struct s_rgb
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
}					t_rgb;

typedef struct s_triangle
{
	int				tip_x;
	int				tip_y;
	int				left_x;
	int				left_y;
	int				right_x;
	int				right_y;
	float			dir_x;
	float			dir_y;
}					t_triangle;

typedef struct s_line
{
	t_coord			current;
	t_coord			end;
	int				dx;
	int				dy;
	int				sx;
	int				sy;
	int				err;
	int				e2;
	int				done;
}					t_line;

typedef struct s_minimap
{
	int				radius;
	int				center_x;
	int				center_y;
	double			scale_factor;
	double			max_view_distance;
	double			ratio;
}					t_minimap;

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
	double			run;
}					t_player;

typedef struct s_wall
{
	double			wall_distance;
	double			wall_x;
	int				tex_x;
	int				tex_y;
	double			step;
	double			tex_pos;
	int				line_height;
	int				draw_start;
	int				draw_end;
	int				is_portal;
	t_texture		*tex;
}					t_wall;

typedef struct s_ray_floor
{
	float			pos_z;
	float			row_distance;
	float			floor_step_x;
	float			floor_step_y;
	float			floor_x;
	float			floor_y;
}					t_ray_floor;

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
	t_wall			wall;
}					t_raycasting;

typedef struct s_map
{
	int				width;
	int				height;
	int				floor_color;
	int				ceiling_color;
	int				**matrix;
	int				nb_cores;
	char			orientation_start;
	t_coord			player_start;
}					t_map;

typedef struct s_mlx
{
	void			*mlx;
	void			*win;
}					t_mlx;

typedef struct s_fps
{
	int				frames;
	int				last_time;
	int				delta_time;
	int				fps;
}					t_fps;

typedef struct s_anim
{
	pthread_t		thread;
	pthread_mutex_t	mutex;
	pthread_mutex_t	stop_mutex;
	t_texture		*frames[NB_FRAMES];
	t_texture		*curr_frame;
	int				frame_i;
	int				stop;
}					t_anim;

typedef struct s_portal
{
	int				x;
	int				y;
	int				old_tex;
	t_map_element	orientation;
}					t_portal;

typedef struct s_data
{
	t_mlx			s_mlx;
	t_player		player;
	t_img			s_img;
	t_map			*map;
	t_texture		*tex[TEX_COUNT];
	t_texture		*gun_tex;
	t_fps			s_fps;
	t_anim			anim;
	int				mouse_off;
	int				max_row;
	t_minimap		minimap;
	int				max_col;
	t_portal		portal1;
	t_portal		portal2;
	int				started;
	struct timeval	start_time;
	long			time_left;
	int				game_end;
	int				first_portal;
	int				show_map;
}					t_data;

#endif
