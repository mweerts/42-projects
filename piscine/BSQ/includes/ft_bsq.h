/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bsq.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 19:59:36 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/25 19:16:51 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_BSQ_H
# define FT_BSQ_H

# define BUFFER_SIZE 4096

# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>

typedef struct s_map
{
	char	empty;
	char	obstacle;
	char	filled;
	int		rows;
	int		cols;
	char	**map;
}	t_map;

typedef struct s_read_data
{
	int		i;
	int		bytes;
	int		col;
	int		row;
	char	*line;
	int		line_len;
}	t_read_data;

typedef struct s_square_coord
{
	int	max;
	int	max_x;
	int	max_y;
}	t_square_coord;

int		ft_solve(t_map *map);
void	ft_print_map(t_map *map);
char	*ft_strndup(char *str, int size);
char	*ft_strcpy(char *dest, char *src);
void	ft_putchar(char c);
void	ft_putstr(char *str);
void	ft_putchar_err(char c);
void	ft_putstr_err(char *str);
int		ft_strlen(char *str);
int		ft_min(int a, int b, int c);
int		ft_init_map(char *read_buffer, t_map **map, int *i, int *first_read);
int		ft_fill_map(char *r_buf, t_map **map, t_read_data *data);
int		ft_malloc_map(t_map **map, int row);
int		ft_malloc_map_grid(t_map **map, int col);
void	ft_free_map(t_map *map);
int		ft_char_is_numeric(char c);
int		ft_isspace(char c);
int		ft_light_atoi(char *str, int n);
int		ft_strlenc(char *str, char c);
char	*ft_realloc(char *old_buf, int new_size);
void	ft_update_coords(int **matrix, int i, int j, t_square_coord *coords);
void	ft_init_coords(t_square_coord *coords);
void	ft_init_data(t_read_data *data);
int		ft_validate_char(char c, t_map *map);
int		ft_different_char(t_map *map);

#endif