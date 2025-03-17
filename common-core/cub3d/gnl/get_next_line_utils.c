/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweerts <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/22 12:18:16 by mweerts           #+#    #+#             */
/*   Updated: 2019/11/25 23:32:26 by mweerts          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(const char *s)
{
	size_t i;

	i = 0;
	while (s && s[i])
		i++;
	return (i);
}

char	*ft_strjoin(const char *s1, const char *s2)
{
	int		i;
	int		j;
	char	*ret;

	i = 0;
	j = 0;
	ret = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (ret == NULL)
		return (NULL);
	while (s1 && s1[i])
	{
		ret[i] = s1[i];
		i++;
	}
	while (s2 && s2[j])
		ret[i++] = s2[j++];
	ret[i] = '\0';
	return (ret);
}

char	*ft_strchr(const char *s, int c)
{
	int		i;
	char	*str;

	if (!s)
		return (NULL);
	i = 0;
	str = (char *)s;
	while (str[i])
	{
		if (str[i] == c)
			return (&str[i]);
		i++;
	}
	if (str[i] == c)
		return (&str[i]);
	return (NULL);
}

char	*ft_strdup(const char *s)
{
	char	*dest;
	int		i;

	i = 0;
	dest = malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (dest == NULL)
		return (NULL);
	while (s && s[i])
	{
		dest[i] = s[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	size_t		i;
	char		*substr;

	i = 0;
	if (s == NULL)
	{
		substr = malloc(sizeof(char));
		if (substr == NULL)
			return (NULL);
		substr[0] = '\0';
		return (substr);
	}
	substr = malloc(sizeof(char) * (len + 1));
	if (substr == NULL)
		return (NULL);
	if (start <= ft_strlen(s))
	{
		while (i < len)
			substr[i++] = s[start++];
	}
	substr[i] = '\0';
	return (substr);
}
