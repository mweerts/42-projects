/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 20:52:20 by apierret          #+#    #+#             */
/*   Updated: 2024/09/25 11:33:28 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_bsq.h"

int	ft_char_is_numeric(char c)
{
	return (c >= '0' && c <= '9');
}

int	ft_isspace(char c)
{
	if (c == '\t' || c == '\n' || c == '\v'
		|| c == '\f' || c == '\r' || c == ' ')
		return (1);
	return (0);
}

int	ft_light_atoi(char *str, int n)
{
	int	i;
	int	value;

	i = 0;
	value = -1;
	while (i < n && ft_isspace(str[i]))
		i++;
	while (i < n && str[i] >= '0' && str[i] <= '9')
	{
		if (value == -1)
			value = 0;
		value *= 10;
		value += str[i] - '0';
		i++;
	}
	return (value);
}

int	ft_strlenc(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	return (i);
}
