/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 12:31:54 by maxweert          #+#    #+#             */
/*   Updated: 2024/10/10 12:39:03 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(const char *s1, const char *s2)
{
	size_t	total_len;
	char	*ret;

	total_len = ft_strlen(s1) + ft_strlen(s2) + 1;
	ret = ft_calloc(total_len, sizeof(char));
	if (!ret)
		return (NULL);
	ft_strlcat(ret, s1, total_len);
	ft_strlcat(ret, s2, total_len);
	return (ret);
}
