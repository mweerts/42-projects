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

void data_init(t_data *data)
{
	ft_memset(data, 0, sizeof(t_data));
	data->mlx = mlx_init();
	if (!data->mlx)
		display_err_and_exit(NULL, data);
	data->map = malloc(sizeof(t_map));
	if (!data->map)
		display_err_and_exit(NULL, data);
}

int main(int ac, char **argv)
{
	t_data data;
	(void)ac;
	
	data_init(&data);
	parse_map(&data, ac, argv);
	return (0);
}