/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_display_file.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 19:52:33 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/19 12:30:20 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_display_file.h"

int	ft_putstr_error(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	write(2, str, i);
	return (-1);
}

int	main(int argc, char **argv)
{
	char	buffer[2];
	int		buffer_size;
	int		fd;
	int		ret;

	if (argc < 2)
		return (ft_putstr_error("File name missing.\n"));
	if (argc > 2)
		return (ft_putstr_error("Too many arguments.\n"));
	fd = open(argv[1], O_RDONLY);
	buffer_size = 2;
	if (fd == -1)
		return (ft_putstr_error("Cannot read file.\n"));
	ret = read(fd, buffer, buffer_size);
	while (ret > 0)
	{
		write(1, buffer, ret);
		ret = read(fd, buffer, buffer_size);
	}
	if (close(fd) == -1)
		return (-1);
	return (0);
}
