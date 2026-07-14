/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 20:25:06 by apierret          #+#    #+#             */
/*   Updated: 2024/09/25 22:00:22 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_bsq.h"

int	ft_load_source(int input, t_map **map)
{
	char		read_buffer[BUFFER_SIZE + 1];
	int			first_read;
	int			ret;
	t_read_data	data;

	first_read = 1;
	ft_init_data(&data);
	while (1)
	{
		data.bytes = read(input, read_buffer, BUFFER_SIZE);
		read_buffer[data.bytes] = '\0';
		if (data.bytes == 0)
			break ;
		data.i = 0;
		if (first_read)
		{
			ret = ft_init_map(read_buffer, map, &data.i, &first_read);
			if (!ret)
				return (ret);
		}
		ret = ft_fill_map(read_buffer, map, &data);
		if (!ret)
			return (ret);
	}
	return (data.row >= (*map)->rows);
}

int	ft_load_file(char *file, t_map **map)
{
	int	fd;
	int	ret;

	fd = open(file, O_RDONLY);
	if (fd == -1)
		return (0);
	ret = ft_load_source(fd, map);
	close(fd);
	return (ret);
}

void	ft_solve_stdin(t_map **map)
{
	int	ret;

	ret = ft_load_source(0, map);
	if (ret != 1)
		ft_putstr_err("map error\n");
	else
		ft_solve(*map);
}

int	main(int argc, char *argv[])
{
	int		ret;
	t_map	*map;
	int		i;

	i = 1;
	if (argc == 1)
		ft_solve_stdin(&map);
	else
	{
		while (i < argc)
		{
			ret = ft_load_file(argv[i], &map);
			if (ret)
				ft_solve(map);
			else
				ft_putstr_err("map error\n");
			if (i < argc - 1)
				ft_putchar('\n');
			i++;
		}
	}
	return (0);
}
