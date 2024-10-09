/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 20:31:02 by maxweert          #+#    #+#             */
/*   Updated: 2024/10/09 20:33:40 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		i;
	char	chr;

	i = ft_strlen(s);
	chr = (char)c;
	while (i >= 0)
	{
		if (s[i] == chr)
			return ((char *)&s[i]);
		i--;
	}
	return (0);
}
