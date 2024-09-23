/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_string.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 13:45:32 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/22 15:15:34 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rush02.h"

char	*ft_parse_string(char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i] == ' ')
		i++;
	while (ft_char_is_printable(str[i + j]))
		j++;
	j--;
	while (str[i + j] == ' ')
		j--;
	j++;
	return (ft_strndup(&str[i], j));
}
