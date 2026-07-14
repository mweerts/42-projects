/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_range.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:35:21 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/11 18:50:57 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

int	*ft_range(int min, int max)
{
	int	*ret;
	int	i;

	if (min >= max)
		return (0);
	ret = (int *)malloc((max - min) * sizeof(int));
	if (!ret)
		return (0);
	i = 0;
	while (i < max - min)
	{
		ret[i] = min + i;
		i++;
	}
	return (ret);
}
