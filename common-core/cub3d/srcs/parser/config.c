/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:57:10 by llebugle          #+#    #+#             */
/*   Updated: 2025/03/24 23:39:23 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static int	get_rgb_color(char *line)
{
	int		color;
	char	**rgb;

	color = 0;
	if (!line)
		return (-1);
	rgb = ft_split(line, ',');
	if (!rgb)
		return (-1);
	color = rgb_to_int(rgb);
	ft_free_tab(rgb);
	if (color < 0)
		return (-1);
	return (color);
}

int	validate_config(t_data *data)
{
	int	i;

	i = 0;
	while (i < TEX_COUNT)
	{
		if (!data->tex[i])
			return (print_err(MSG_MISSING_TEX), ERROR);
		i++;
	}
	if (data->map->floor_color < 0 || data->map->ceiling_color < 0)
		return (print_err(MSG_ERR_RGB), ERROR);
	return (0);
}

static int	offset(char *line)
{
	int	offset;

	offset = 0;
	if (ft_strncmp(line, "NO ", 3) == 0)
		offset = 2;
	else if (ft_strncmp(line, "SO ", 3) == 0)
		offset = 2;
	else if (ft_strncmp(line, "WE ", 3) == 0)
		offset = 2;
	else if (ft_strncmp(line, "EA ", 3) == 0)
		offset = 2;
	else if (ft_strncmp(line, "F ", 2) == 0)
		offset = 1;
	else if (ft_strncmp(line, "C ", 2) == 0)
		offset = 1;
	while (line[offset] == ' ' || line[offset] == '\t')
		offset++;
	return (offset);
}

bool	get_textures_and_colors(t_data *data, char *line)
{
	int	i;

	i = 0;
	if (!line)
		return (0);
	if (ft_strncmp(line, "NO ", 3) == 0)
		data->tex[TEX_NORTH] = load_texture(data->s_mlx.mlx, line + offset(line));
	else if (ft_strncmp(line, "SO ", 3) == 0)
		data->tex[TEX_SOUTH] = load_texture(data->s_mlx.mlx, line + offset(line));
	else if (ft_strncmp(line, "WE ", 3) == 0)
		data->tex[TEX_WEST] = load_texture(data->s_mlx.mlx, line + offset(line));
	else if (ft_strncmp(line, "EA ", 3) == 0)
		data->tex[TEX_EAST] = load_texture(data->s_mlx.mlx, line + offset(line));
	else if (ft_strncmp(line, "F ", 2) == 0)
		data->map->floor_color = get_rgb_color(line + offset(line));
	else if (ft_strncmp(line, "C ", 2) == 0)
		data->map->ceiling_color = get_rgb_color(line + offset(line));
	else
		return (DONE);
	return (0);
}

int	parse_config(t_data *data, char *line)
{
	char	*trimmed;
	int		status;

	if (!line || ft_strcmp(line, "\n") == 0)
		return (0);
	trimmed = ft_strtrim(line, " \t\n");
	if (!trimmed)
		return (-1);
	status = get_textures_and_colors(data, trimmed);
	free(trimmed);
	return (status);
}

