/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structures.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:05:30 by llebugle          #+#    #+#             */
/*   Updated: 2025/03/13 19:05:32 by llebugle         ###   ########.fr       */
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
	void			*mlx;
	void			*win;
	t_img			img;
	t_map			*map;
	t_texture		*tex[TEX_COUNT];
	int				max_row;
	int				max_col;
	// char		*err_msg;
}					t_data;

#endif

