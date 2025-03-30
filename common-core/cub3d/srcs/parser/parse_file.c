/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:57:32 by llebugle          #+#    #+#             */
/*   Updated: 2025/03/27 00:16:49 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	check_arguments(int ac, char **av)
{
	int	len;

	if (ac < 2)
		return (print_err("The map is missing.\n"), ERROR);
	if (ac > 2)
		return (print_err("Too many arguments.\n"), ERROR);
	len = ft_strlen(av[1]);
	if (len < 4 || !ft_strnstr(&av[1][len - 4], ".cub", 4))
		return (print_err(MSG_WRONG_EXTENSION), ERROR);
	return (0);
}

int	parse_file(t_data *data, char **line)
{
	int	i;
	int	config_done;

	i = 0;
	config_done = false;
	while (line && line[i])
	{
		config_done = parse_config(data, line[i]);
		if (config_done == -1)
			return (ERROR);
		if (config_done)
			break ;
		i++;
	}
	data->tex[TEX_FLOOR] = load_texture(data->s_mlx.mlx, "./assets/floor.xpm");
	data->tex[TEX_CEILING] = load_texture(data->s_mlx.mlx,
			"./assets/ceiling.xpm");
	if (get_map_size(data->map, line + i) == ERROR)
		return (ERROR);
	if (create_matrix(data->map, line + i) == ERROR)
		return (ERROR);
	if (is_map_closed(data->map) || validate_config(data))
		return (ERROR);
	return (0);
}

char	**read_file(int fd)
{
	char	*line;
	char	*file;
	char	**tab;

	file = ft_strdup("");
	if (!file)
		return (NULL);
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		file = ft_strjoin_n_free(file, line);
		if (!file)
			return (free(line), NULL);
		free(line);
	}
	tab = ft_split(file, '\n');
	free(file);
	return (tab);
}

static int	process_file(t_data *data, char *filepath)
{
	int		fd;
	char	**tab;

	fd = open(filepath, O_RDONLY);
	if (fd < 0)
		return (print_err(ERR_NO_MAP), ERROR);
	tab = read_file(fd);
	if (!tab)
		return (close(fd), ERROR);
	if (parse_file(data, tab) == ERROR)
		return (ft_free_tab(tab), close(fd), ERROR);
	close(fd);
	ft_free_tab(tab);
	return (0);
}

int	parse_arguments(t_data *data, int ac, char **av)
{
	data->gun_tex = load_texture(data->s_mlx.mlx, "./assets/portal_gun.xpm");
	if (check_arguments(ac, av) == ERROR)
		return (clean_up(data), exit(ERROR), ERROR);
	data->max_col = WIDTH;
	data->max_row = HEIGHT;
	if (process_file(data, av[1]) == ERROR)
	{
		clean_up(data);
		exit(ERROR);
	}
	return (0);
}
