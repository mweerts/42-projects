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

int	leave(t_data *data)
{
	data->anim.stop = 1;
	pthread_mutex_lock(&data->anim.stop_mutex);
	pthread_mutex_unlock(&data->anim.stop_mutex);
	ft_usleep(50);
	clean_up(data);
	exit(0);
}

int	main(int ac, char **av)
{
	t_data data;
	
	if (ac != 2)
	{
		ft_printf_fd(2, "Error\nUsage: ./cub3d <map_file.cub>\n");
		return (1);
	}
	init_data(&data);
	init_mlx(&data.s_mlx);
	if (parse_arguments(&data, ac, av))
		exit_with_error(NULL, &data);
	clean_up(&data);
	return (0);
}