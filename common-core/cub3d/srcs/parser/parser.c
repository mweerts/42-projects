/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:57:32 by llebugle          #+#    #+#             */
/*   Updated: 2025/03/24 23:39:04 by maxweert         ###   ########.fr       */
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

int	parse_arguments(t_data *data, int ac, char **av)
{
	if (check_arguments(ac, av) == ERROR)
		return (clean_up(data), exit(ERROR), ERROR);
	mlx_get_screen_size(data->s_mlx.mlx, &(data->max_col), &(data->max_row));
	if (process_file(data, av[1]) == ERROR)
	{
		clean_up(data);
		exit(ERROR);
	}
	return (0);
}
