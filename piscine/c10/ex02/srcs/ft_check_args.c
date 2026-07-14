/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 16:43:32 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/19 17:01:37 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_tail.h"

int	ft_str_is_numeric(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return (0);
		i++;
	}
	return (1);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			break ;
		i++;
	}
	return (s1[i] - s2[i]);
}

int	ft_check_args(int argc, char **argv)
{
	if (argc < 2)
		return (0);
	if (argc == 2 && ft_strcmp(argv[1], "-c") != 0)
		return (0);
	if (ft_strcmp(argv[1], "-c") == 0)
	{
		if (argc >= 3 && ft_str_is_numeric(argv[2]))
			return (1);
		else
		{
			ft_putstr(2, "tail: illegal offset -- ");
			ft_putstr(2, argv[2]);
			ft_putstr(2, "\n");
			return (0);
		}
	}
	return (0);
}
