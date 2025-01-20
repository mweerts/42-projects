/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 12:41:05 by maxweert          #+#    #+#             */
/*   Updated: 2025/01/07 18:37:51 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(const char *s1, const char *set)
{
	size_t	i;
	size_t	j;
	char	*ret;

	i = 0;
	j = ft_strlen(s1);
	while (s1[i] && ft_isinset(s1[i], set))
		i++;
	while (j > 0 && ft_isinset(s1[j - 1], set))
		j--;
	ret = ft_substr(s1, i, j - i);
	return (ret);
}
