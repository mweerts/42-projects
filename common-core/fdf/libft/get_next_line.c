/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:21:55 by maxweert          #+#    #+#             */
/*   Updated: 2025/01/17 17:57:22 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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
	tmp = *line;
	*line = ft_substr(tmp, 0, i + 1);
	free(tmp);
	if (!(*line))
	{
		free(left);
		left = NULL;
		return (NULL);
	}
	return (left);
}

static char	*read_fd(int fd, char *left, char *buffer)
{
	char	*tmp;
	int		rd;

	while (1)
	{
		rd = read(fd, buffer, BUFFER_SIZE);
		if (rd < 0)
		{
			free(left);
			return (NULL);
		}
		if (rd == 0)
			break ;
		buffer[rd] = '\0';
		if (!left)
			left = ft_calloc(1, sizeof(char));
		if (!left)
			return (NULL);
		tmp = ft_strjoin(left, buffer);
		free(left);
		left = tmp;
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	return (left);
}

char	*get_next_line(int fd)
{
	static char	*left[FD_MAX];
	char		*line;
	char		buffer[BUFFER_SIZE + 1];

	if (fd < 0)
		return (NULL);
	line = read_fd(fd, left[fd], buffer);
	left[fd] = set_line(&line);
	if (!line || line[0] == '\0')
	{
		free(left[fd]);
		left[fd] = NULL;
		return (NULL);
	}
	return (line);
}
