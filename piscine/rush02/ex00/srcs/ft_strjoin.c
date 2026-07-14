/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 21:02:26 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/22 17:32:39 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
	This is a modified version of ft_strjoin.
	The function takes two strings as argument, then return
	the concatenation of the two strings. (malloc)
	It also free the first argument.
*/

#include "rush02.h"

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

char	*ft_strjoin(char *s1, char *s2)
{
	char	*ret;
	int		total_size;

	total_size = 0;
	if (s1 && s1[0])
		total_size += ft_strlen(s1);
	total_size += ft_strlen(s2);
	ret = (char *)malloc((total_size + 1) * sizeof(char));
	*ret = 0;
	if (!ret)
		return (0);
	if (!s1)
		ret = ft_strcpy(ret, s2);
	else
	{
		ft_strcat(ret, s1);
		ft_strcat(ret, s2);
	}
	if (s1)
		free(s1);
	return (ret);
}
