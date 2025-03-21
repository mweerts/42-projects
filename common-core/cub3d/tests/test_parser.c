/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:30:15 by llebugle          #+#    #+#             */
/*   Updated: 2025/03/13 19:30:16 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	main(int ac, char **av)
{
	t_data data;
	
	if (ac != 2)
	{
		ft_printf_fd(2, "Error\nUsage: ./cub3d <map_file.cub>\n");
		return (1);
	}
	data_init(&data);
	if (parse_arguments(&data, ac, av))
		exit_with_error(NULL, &data);
	clean_up(&data);
	return (0);
}