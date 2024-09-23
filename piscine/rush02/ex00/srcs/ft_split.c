/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 12:30:50 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/22 13:44:02 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rush02.h"

int	ft_char_is_in_charset(char c, char *charset)
{
	int	i;

	i = 0;
	while (charset[i])
	{
		if (c == charset[i])
			return (1);
		i++;
	}
	return (0);
}

char	*ft_get_next_elem(char *str, char *charset)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (ft_char_is_in_charset(str[i], charset))
		{
			while (str[i] && ft_char_is_in_charset(str[i], charset))
				i++;
			if (str[i] == '\0')
				return (0);
			else
				return (&str[i]);
		}
		i++;
	}
	return (0);
}

int	ft_count_words(char *str, char *charset)
{
	int		count;
	char	*next;
	int		i;

	count = 0;
	next = str;
	while (next != 0)
	{
		i = 0;
		while (str[i] && !ft_char_is_in_charset(str[i], charset))
			i++;
		if (i > 0)
			count++;
		next = ft_get_next_elem(next, charset);
	}
	return (count);
}

char	**ft_split(char *str, char *charset)
{
	char	**ret;
	int		i;
	int		j;
	int		size;

	i = 0;
	while (*str && ft_char_is_in_charset(*str, charset))
		str++;
	size = ft_count_words(str, charset);
	ret = (char **)malloc((size + 1) * sizeof(char *));
	if (!ret)
		return (0);
	while (i < size)
	{
		j = 0;
		while (str[j] && !ft_char_is_in_charset(str[j], charset))
			j++;
		ret[i] = ft_strndup(str, j);
		if (!ret[i])
			return (0);
		str = ft_get_next_elem(str, charset);
		i++;
	}
	ret[i] = 0;
	return (ret);
}
