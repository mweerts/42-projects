/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structures.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:05:30 by llebugle          #+#    #+#             */
/*   Updated: 2025/03/23 23:31:08 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTURES_H
# define STRUCTURES_H

typedef struct s_img
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}			t_img;

typedef struct s_mlx
{
	void	*mlx;
	void	*win;
}	t_mlx;

typedef struct s_player
{
	double		pos_x;
	double		pos_y;
	double		dir_x;
	double		dir_y;
	double		plane_x;
	double		plane_y;
	int			mv_forward;
	int			mv_lateral;
	int			mv_rotate;
}	t_player;

typedef struct s_map
{
	int		width;
	int		height;
	int		floor_color;
	int		ceiling_color;
	char	orientation_start;
	int		matrix[12][24];
}	t_map;

typedef struct s_data
{
	t_mlx		s_mlx;
	t_player	player;
	t_map		map;
	t_img		s_img;
}	t_data;

typedef struct s_raycasting
{
	double	camera_x;
	double	ray_dir_x;
	double	ray_dir_y;
	int		ray_x;
	int		ray_y;
	int		hit;
	int		side;
	double	side_dist_x;
	double	side_dist_y;
	double	delta_dist_x;
	double	delta_dist_y;
	int		step_x;
	int		step_y;
	double	wall_distance;
}	t_raycasting;

#endif