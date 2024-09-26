/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 11:06:10 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/23 18:26:56 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_hexdump.h"

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	ft_putstr(int fd, char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	write(fd, str, i);
	if (fd == 2)
		return (-1);
	return (0);
}

int	ft_putstr_error(char *path, int err, char *prog_name)
{
	ft_putstr(2, basename(prog_name));
	ft_putstr(2, ": ");
	if (path[0])
		ft_putstr(2, basename(path));
	ft_putstr(2, ": ");
	ft_putstr(2, strerror(err));
	ft_putstr(2, "\n");
	return (-1);
}

char	*ft_strncat(char *dest, char *src, int n)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (dest[i])
		i++;
	while (src[j] && j < n)
	{
		dest[i + j] = src[j];
		j++;
	}
	dest[i + j] = '\0';
	return (dest);
}
