/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 17:35:26 by maxweert          #+#    #+#             */
/*   Updated: 2025/01/17 18:55:44 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	get_map_height(char *filename)
{
	int		fd;
	int		height;
	char	*line;

	height = 0;
	if (!ft_strnstr(filename, ".fdf", ft_strlen(filename)))
		ft_error("Wrong file extension.");
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		ft_error("Cannot open map.");
	if (read(fd, NULL, 0) < 0)
		ft_error("Cannot open map.");
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		height++;
		free(line);
	}
	return (height);
}

static int	count_x_points(char *line)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (line[i])
	{
		while (line[i] == ' ')
			i++;
		if (line[i] && line[i] != '\n')
			count++;
		while (line[i] && line[i] != ' ')
			i++;
	}
	return (count);
}

int	get_map_width(char *filename)
{
	int		fd;
	int		width;
	char	*line;
	int		count;

	width = 0;
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		ft_error("Cannot open map.");
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		count = count_x_points(line);
		free(line);
		if (width == 0)
			width = count;
		if (width != count)
			ft_error("Map error.");
	}
	return (width);
}

static void	set_color(t_data *data, int x, int y, char *str)
{
	data->map.z_arr[y][x] = ft_atoi_base(str, "0123456789");
	if (ft_strchr(str, ','))
		data->map.colors_arr[y][x] = ft_atoi_base(ft_strchr(str, ',') + 3,
				"0123456789ABCDEF");
	else
		data->map.colors_arr[y][x] = 0xFFFFFFFF;
}

void	parse_map(t_data *data, char *filename)
{
	int		x;
	int		y;
	int		fd;
	char	*line;
	char	**splited;

	y = 0;
	fd = open(filename, O_RDONLY);
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		splited = ft_split(line, ' ');
		free(line);
		x = 0;
		while (x < data->map.width)
		{
			set_color(data, x, y, splited[x]);
			x++;
		}
		free_tab(splited);
		y++;
	}
}
