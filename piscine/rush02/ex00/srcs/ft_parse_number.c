/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_number.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 14:39:17 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/22 16:32:07 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rush02.h"

char	*ft_trim_number(char *number)
{
	int		i;
	int		j;
	char	*ret;

	i = 0;
	j = 0;
	while (ft_char_isspace(number[i]))
		i++;
	if (number[i] == '+')
		i++;
	while (number[i] == '0' && number[i + 1])
		i++;
	while (number[i + j] >= '0' && number[i + j] <= '9')
		j++;
	ret = ft_strndup(&number[i], j);
	free(number);
	return (ret);
}

int	ft_check_str_number(char *number)
{
	int	i;

	i = 0;
	if (!(ft_char_isspace(number[0]) || number[0] == '+'
			|| number[0] == '-' || (number[0] >= '0' && number[0] <= '9')))
		return (0);
	while (ft_char_isspace(number[i]))
		i++;
	if (number[i] == '-')
		return (2);
	if (number[i] == '+')
		i++;
	if (!(number[i] >= '0' && number[i] <= '9'))
		return (0);
	while (number[i] >= '0' && number[i] <= '9')
		i++;
	return (1);
}

int	ft_str_number_is_multiplier(char *number)
{
	int	i;
	int	count_zeros;

	i = 1;
	count_zeros = 0;
	if (ft_strlen(number) <= 3)
		return (0);
	if (number[0] != '1')
		return (0);
	while (number[i])
	{
		if (number[i] != '0')
			return (0);
		count_zeros++;
		i++;
	}
	if (count_zeros % 3 == 0)
		return (1);
	return (0);
}

int	ft_get_multiplier(char *multiplier)
{
	int	i;

	i = 1;
	while (multiplier[i] == '0')
		i++;
	return (i / 3);
}
