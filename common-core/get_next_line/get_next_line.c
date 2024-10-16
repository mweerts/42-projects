/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:21:55 by maxweert          #+#    #+#             */
/*   Updated: 2024/10/16 23:31:22 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "get_next_line.h"

char	*read_line(int fd, char *left, char *buffer)
{

}

char	*set_line(char *line)
{

}

char	*get_next_line(int fd)
{
	static char	*left;
	char		*buffer;
	char		*line;

	buffer = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	return (line);
}

int main()
{
	int		fd;
	char	*line;

	fd = -1;
	line = get_next_line(fd);
	fd = open("read_error.txt", O_RDONLY);
	line = get_next_line(fd);
	while (line)
	{
		printf("%s", line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}