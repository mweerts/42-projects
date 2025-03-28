/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 18:40:30 by llebugle          #+#    #+#             */
/*   Updated: 2025/03/28 00:11:11 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	parse_map(t_data *data, char **line)
{
	if (!line || !line[0])
		return (ERROR);
	if (get_map_size(data->map, line))
		return (ERROR);
	if (create_matrix(data->map, line))
		return (ERROR);
	if (is_map_closed(data->map))
		return (ERROR);
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
	if (validate_config(data) == ERROR)
		return (ERROR);
	if (parse_map(data, line + i) == ERROR)
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

int	process_file(t_data *data, char *filepath)
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
