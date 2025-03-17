/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweerts <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/22 15:54:18 by mweerts           #+#    #+#             */
/*   Updated: 2019/11/26 12:50:54 by mweerts          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	get_line(char **str, char **line)
{
	int		i;
	char	*tmp;

	i = 0;
	if (!(*str) || !str || !line)
		return (-1);
	while ((*str)[i] != '\n' && (*str)[i] != '\0')
		i++;
	if (!(*line = ft_substr(*str, 0, i)))
		return (-1);
	if ((*str)[i] == '\n')
	{
		tmp = *str;
		*str = ft_strdup(&tmp[i + 1]);
		free(tmp);
		if (!(*str))
			return (-1);
	}
	else if ((*str)[i] == '\0')
	{
		free(*str);
		*str = NULL;
	}
	return (1);
}

static int	ft_read(int fd, char **tab)
{
	char	*buff;
	char	*tmp;
	int		ret;

	if (!(buff = malloc(sizeof(char) * (BUFFER_SIZE + 1))))
		return (-1);
	while ((ret = read(fd, buff, BUFFER_SIZE)) > 0)
	{
		buff[ret] = '\0';
		if (tab[fd])
		{
			tmp = tab[fd];
			tab[fd] = ft_strjoin(tmp, buff);
			free(tmp);
			tmp = NULL;
		}
		else
			tab[fd] = ft_strdup(buff);
		if (ft_strchr(tab[fd], '\n'))
			break ;
	}
	free(buff);
	return (ret);
}

int			get_next_line(int fd, char **line)
{
	static char	*tab[OPEN_MAX + 1];
	int			ret;

	if (fd < 0 || fd > OPEN_MAX || BUFFER_SIZE < 1 || !line)
		return (-1);
	if (tab[fd] && ft_strchr(tab[fd], '\n'))
		return (get_line(&tab[fd], line));
	if ((ret = ft_read(fd, tab)) < 0)
		return (-1);
	if (ret == 0)
	{
		*line = ft_strdup(tab[fd]);
		if (tab[fd])
		{
			free(tab[fd]);
			tab[fd] = NULL;
		}
		if (!(*line))
			return (-1);
		return (0);
	}
	return (get_line(&tab[fd], line));
}
