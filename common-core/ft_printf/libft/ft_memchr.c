/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 20:50:08 by maxweert          #+#    #+#             */
/*   Updated: 2024/10/10 12:17:13 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*tmp;
	unsigned char	chr;

	i = 0;
	tmp = (unsigned char *)s;
	chr = (unsigned char)c;
	while (i < n)
	{
		if (tmp[i] == chr)
			return ((void *)&tmp[i]);
		i++;
	}
	return (NULL);
}
