/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:37:37 by maxweert          #+#    #+#             */
/*   Updated: 2024/10/10 14:03:29 by maxweert         ###   ########.fr       */
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

char	**ft_split(const char *s, char c)
{
	size_t	i;
	size_t	j;
	size_t	k;
	size_t	tab_len;
	char	**ret;

	i = 0;
	j = 0;
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
		k++;
	}
	return (ret);
}

// #include <stdio.h>
// int main()
// {
// 	char **tab;

// 	tab = ft_split(" test       efwffeewfwef       salut     ", ' ');
// 	for (int i =0; tab[i]; i++)
// 		printf("%s\n", tab[i]);
// }