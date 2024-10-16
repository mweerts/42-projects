/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:21:55 by maxweert          #+#    #+#             */
/*   Updated: 2024/10/16 17:10:57 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "get_next_line.h"

char	*ft_strchr(const char *s, int c)
{
	size_t	i;
	char	chr;

	i = 0;
	chr = (char)c;
	while (s[i])
	{
		if (s[i] == chr)
			return ((char *)&s[i]);
		i++;
	}
	if (s[i] == chr)
		return ((char *)&s[i]);
	return (NULL);
}

char	*read_line(int fd, char *left, char *buffer)
{
	char	*tmp;
	int		rd;

	if (left && ft_strchr(left, '\n'))
		return (left);
	rd = read(fd, buffer, BUFFER_SIZE);
	
	while (rd > 0)
	{
		printf("rd : %d\n", rd);
		buffer[rd] = '\0';
		if (!left)
			left = ft_calloc(1, sizeof(char));
		tmp = left;
		left = ft_strjoin(left, buffer);
		free(tmp);
		if (ft_strchr(buffer, '\n'))
			return (left);
		rd = read(fd, buffer, BUFFER_SIZE);
	}
	if (rd == 0 && left)
		return (left);
	return (NULL);
}

char	*get_line(char *line)
{
	char	*left = NULL;
	size_t	i;

	if (!line)
		return (NULL);
	i = 0;
	while (line[i] && line[i] != '\n')
		i++;
	if (line[i] == 0)
		left = line;
	else
	{
		left = ft_substr(line, i + 1, ft_strlen(line) - i);
		line[i + 1] = '\0';
	}
	return (left);
}

char	*get_next_line(int fd)
{
	static char	*left = NULL;
	char		buffer[BUFFER_SIZE + 1];
	char		*line = NULL;

	line = read_line(fd, left, buffer);
	if (!line)
		return (NULL);
	left = get_line(line);
	return (line);
}

int main()
{
	int		fd;
	int		i;
	char	*line;

	fd = open("test", O_RDONLY);
	line = get_next_line(fd);
	i = 0;
	while (line)
	{
		printf("[fd : %d line : %d]%s", fd, i, line);
		i++;
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}