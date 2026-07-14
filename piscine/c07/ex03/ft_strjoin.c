/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 20:23:24 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/10 12:26:11 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strcat(char *dest, char *src)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (dest[i])
		i++;
	while (src[j])
	{
		dest[i] = src[j];
		j++;
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strjoin(int size, char **strs, char *sep)
{
	char	*ret;
	int		total_size;
	int		i;

	total_size = 0;
	i = 0;
	while (i < size)
	{
		total_size += ft_strlen(strs[i]);
		if (i + 1 < size)
			total_size += ft_strlen(sep);
		i++;
	}
	ret = (char *)malloc((total_size + 1) * sizeof(char));
	*ret = 0;
	if (!ret)
		return (0);
	i = 0;
	while (i < size)
	{
		ft_strcat(ret, strs[i]);
		if (i++ + 1 < size)
			ft_strcat(ret, sep);
	}
	return (ret);
}
