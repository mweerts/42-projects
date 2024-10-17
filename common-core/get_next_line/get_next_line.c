/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:21:55 by maxweert          #+#    #+#             */
/*   Updated: 2024/10/17 13:50:46 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include "get_next_line.h"

static char	*set_line(char **line)
{
	char	*left;
	char	*tmp;
	size_t	i;

	if (!(*line))
		return (NULL);
	i = 0;
	while ((*line)[i] && (*line)[i] != '\n')
		i++;
	if ((*line)[i] == '\0' || (*line)[i + 1] == '\0')
		return (NULL);
	left = ft_substr(*line, i + 1, ft_strlen(*line) - i);
	if (!left)
	{
		*line = NULL;
		return (NULL);
	}
	tmp = *line;
	*line = ft_substr(tmp, 0, i + 1);
	free(tmp);
	if (!(*line))
		return (NULL);
	return (left);
}

static char	*read_fd(int fd, char *left, char *buffer)
{
	char	*tmp;
	int		rd;

	if (left && ft_strchr(left, '\n'))
		return (left);
	while (1)
	{
		rd = read(fd, buffer, BUFFER_SIZE);
		if (rd < 0)
		{
			free(left);
			left = NULL;
			return (NULL);
		}
		if (rd == 0)
			break ;
		buffer[rd] = '\0';
		if (!left)
			left = ft_calloc(1, sizeof(char));
		if (!left)
			return (NULL);
		tmp = left;
		left = ft_strjoin(left, buffer);
		free(tmp);
		tmp = NULL;
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	return (left);
}

char	*get_next_line(int fd)
{
	static char	*left;
	char		*line;
	char		*buffer;

	if (fd < 0)
	{
		free(left);
		left = NULL;
		return (NULL);
	}
	buffer = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (!buffer)
		return (NULL);
	line = read_fd(fd, left, buffer);
	free(buffer);
	buffer = NULL;
	left = set_line(&line);
	if (!line || line[0] == '\0')
		return (NULL);
	return (line);
}

// int main()
// {
// 	int		fd;
// 	char	*line;

// 	fd = open("multiple_nl.txt", O_RDONLY);
// 	line = get_next_line(fd);
// 	while (line)
// 	{
// 		printf("%s", line);
// 		line = get_next_line(fd);
// 	}
// 	close(fd);
// 	return (0);
// }