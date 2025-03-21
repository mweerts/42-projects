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

// Function to validate a map file
// int	validate_map_file(char *filename)
// {
// 	int		fd;
// 	char	*line;
// 	int		result;

// 	result = 0;
// 	fd = open(filename, O_RDONLY);
// 	if (fd < 0)
// 	{
// 		ft_printf_fd(2, "Error\nCould not open file: %s\n", filename);
// 		return (1);
// 	}
// 	result = parse_arguments(t_data *data, int ac, char **av)
// 	// TODO: Add actual validation logic here
// 	// This is a placeholder - real logic would check:
// 	// - Map file has valid texture paths
// 	// - Map has proper wall enclosure
// 	// - Map has exactly one player
// 	// - Colors are valid
// 	// - No invalid characters

// 	// For now, we'll just signal an error for all "invalid" maps
// 	// In a real implementation, this would parse the map and validate its content

// 	close(fd);
// 	return (result);
// }

int	main(int ac, char **av)
{
	t_data data;
	
	if (ac != 2)
	{
		ft_printf_fd(2, "Error\nUsage: %s <map_file.cub>\n", av[0]);
		return (1);
	}
	data_init(&data);
	if (parse_arguments(&data, ac, av))
		exit_with_error(NULL, &data);
	// Validate the map file
	// if (validate_map_file(av[1]) != 0)
	// {
	// 	ft_printf_fd(2, "Error\nInvalid map file: %s\n", av[1]);
	// 	return (1);
	// }

	ft_printf("Map file is valid: %s\n", av[1]);
	return (0);
}