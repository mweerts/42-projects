/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 18:30:02 by maxweert          #+#    #+#             */
/*   Updated: 2025/01/20 03:31:17 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	main(int argc, char **argv)
{
	t_data	data;
	int		fd;

	if (argc != 2)
		ft_error("Wrong number of arguments.");
	if (!ft_strnstr(argv[1], ".fdf", ft_strlen(argv[1])))
		ft_error("Wrong file extension.");
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
		ft_error("Cannot open map.");
	if (read(fd, NULL, 0) < 0)
		ft_error("Cannot open map.");
	init_data(&data, argv[1]);
	draw_map(&data);
	mlx_loop(data.mlx);
	return (0);
}
