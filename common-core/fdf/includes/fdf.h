/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 18:13:49 by maxweert          #+#    #+#             */
/*   Updated: 2025/01/19 18:21:47 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include "libft.h"
# include "mlx.h"
# include "key.h"
# include <fcntl.h>
# include <math.h>

# define WIDTH 1920
# define HEIGHT 1080
# define BACKGROUND_COLOR 0x1F2124
# define MENU_COLOR 0x181A1C
# define MENU_WIDTH 200

typedef enum s_projection
{
	ISOMETRIC,
	PARALLEL
}	t_projection;

typedef struct s_img
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}			t_img;

typedef struct s_line
{
	int		dx;
	int		dy;
	int		steps;
	float	x_inc;
	float	y_inc;
	int		color;
}			t_line;

typedef struct s_coord
{
	int	x;
	int	y;
	int	z;
	int	color;
}	t_coord;

typedef struct s_map
{
	int	width;
	int	height;
	int	**z_arr;
	int	**colors_arr;
}	t_map;

typedef struct s_cam
{
	t_projection	projection;
	int				zoom;
	double			x_rot;
	double			y_rot;
	double			z_rot;
	int				x_offset;
	int				y_offset;
	float			z_divisor;
}	t_cam;

typedef struct s_data
{
	void	*mlx;
	void	*win;
	t_img	img;
	t_map	map;
	t_cam	cam;
}			t_data;

void		init_data(t_data *data, char *filename);
int			leave(t_data *data);
int			key_pressed(int keycode, t_data *data);
void		ft_error(char *str);
void		img_pixel(t_data *data, int x, int y, int color);
void		img_line(t_data *data, t_coord p1, t_coord p2);
void		draw_map(t_data *data);
void		draw_menu(void *mlx, void *win);
t_coord		project(t_data *data, int x, int y);
int			get_map_height(char *filename);
int			get_map_width(char *filename);
void		parse_map(t_data *data, char *filename);
void		move(t_data *data, int keycode);
void		zoom(t_data *data, int keycode);
void		rotate(t_data *data, int keycode);
void		flatten(t_data *data, int keycode);
void		reset(t_data *data);
int			ft_atoi_base(char *str, char *base);
void		free_tab(char **tab);

#endif