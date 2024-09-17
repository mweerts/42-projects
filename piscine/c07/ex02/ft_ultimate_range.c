/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ultimate_range.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 20:02:43 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/09 20:22:45 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

int	ft_ultimate_range(int **range, int min, int max)
{
	int	*ret;
	int	i;

	if (min >= max)
	{
		*range = 0;
		return (0);
	}
	ret = (int *)malloc((max - min) * sizeof(int));
	if (!ret)
		return (-1);
	i = 0;
	while (i < max - min)
	{
		ret[i] = min + i;
		i++;
	}
	*range = ret;
	return (i);
}
