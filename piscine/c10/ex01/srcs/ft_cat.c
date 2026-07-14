/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cat.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 19:52:33 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/19 19:49:17 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_cat.h"

int	ft_cat(char *path, char *buffer, int buffer_size, char *prog_name)
{
	int	fd;
	int	ret;

	if (path)
		fd = open(path, O_RDONLY);
	else
		fd = 0;
	if (fd == -1)
		return (ft_putstr_error(path, errno, prog_name));
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

int	main(int argc, char **argv)
{
	char	buffer[256];
	int		buffer_size;
	int		i;

	i = 1;
	buffer_size = 256;
	if (argc == 1)
	{
		ft_cat(0, buffer, buffer_size, argv[0]);
	}
	while (i < argc)
	{
		ft_cat(argv[i], buffer, buffer_size, argv[0]);
		i++;
	}
	return (0);
}
