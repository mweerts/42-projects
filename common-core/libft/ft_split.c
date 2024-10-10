/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:37:37 by maxweert          #+#    #+#             */
/*   Updated: 2024/10/10 15:56:35 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_count_words(const char *s, char c)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i])
			count++;
		while (s[i] && s[i] != c)
			i++;
	}
	return (count);
}

static char	**ft_free_tab(char **tab, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	return (NULL);
}

char	**ft_split(const char *s, char c)
{
	size_t	i;
	size_t	j;
	size_t	k;
	size_t	tab_len;
	char	**ret;

	i = 0;
	k = 0;
	tab_len = ft_count_words(s, c);
	ret = ft_calloc(tab_len + 1, sizeof(char *));
	if (!ret)
		return (NULL);
	while (k < tab_len)
	{
		while (s[i] == c)
			i++;
		j = i;
		while (s[i] && s[i] != c)
			i++;
		ret[k] = ft_substr(s, j, i - j);
		if (!ret[k])
			return (ft_free_tab(ret, k));
		k++;
	}
	return (ret);
}
