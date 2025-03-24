/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:57:32 by llebugle          #+#    #+#             */
/*   Updated: 2025/03/14 15:57:36 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	check_arguments(int ac, char **av, t_data *data)
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
	if (check_arguments(ac, av, data) == ERROR)
		return (clean_up(data), exit(ERROR), ERROR);
	mlx_get_screen_size(data->mlx, &(data->max_col), &(data->max_row));
	if (process_file(data, av[1]) == ERROR)
	{
		clean_up(data);
		exit(ERROR);
	}
	return (0);
}
