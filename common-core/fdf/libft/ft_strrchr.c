/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 20:31:02 by maxweert          #+#    #+#             */
/*   Updated: 2024/10/10 14:54:33 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	i;
	char	chr;
	char	*ret;

	i = 0;
	chr = (char)c;
	ret = NULL;
	while (s[i])
	{
		if (s[i] == chr)
			ret = (char *)&s[i];
		i++;
	}
	if (s[i] == chr)
		ret = (char *)&s[i];
	return (ret);
}
