/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strs_to_tab.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 20:12:06 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/11 11:31:41 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_stock_str.h"
#include <stdlib.h>

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strdup(char *str)
{
	char	*ret;
	int		i;

	i = 0;
	ret = (char *)malloc((ft_strlen(str) + 1) * sizeof(char));
	if (!ret)
		return (0);
	while (str[i])
	{
		ret[i] = str[i];
		i++;
	}
	ret[i] = '\0';
	return (ret);
}

struct s_stock_str	*ft_strs_to_tab(int ac, char **av)
{
	struct s_stock_str	*ret;
	int					i;

	i = 0;
	ret = (struct s_stock_str *)malloc((ac + 1) * sizeof(struct s_stock_str));
	if (!ret)
		return (0);
	while (i < ac)
	{
		ret[i].size = ft_strlen(av[i]);
		ret[i].str = av[i];
		ret[i].copy = ft_strdup(av[i]);
		i++;
	}
	ret[i].str = 0;
	return (ret);
}
