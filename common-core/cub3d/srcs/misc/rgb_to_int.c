/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rgb_to_int.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 21:21:02 by llebugle          #+#    #+#             */
/*   Updated: 2025/03/14 21:21:06 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	rgb_to_int(char **rgb)
{
	int	r;
	int	g;
	int	b;
	int	i;
	int	j;

	i = 0;
	if (!rgb || !rgb[0] || !rgb[1] || !rgb[2])
		return (-1);
	while (rgb[i])
	{
		j = 0;
		rgb[i] = ft_strtrim_n_free(rgb[i], " \t");
		while (rgb[i][j])
			if (ft_isdigit(rgb[i][j++]) == false)
				return (-1);
		i++;
	}
	r = ft_atoi(rgb[0]);
	g = ft_atoi(rgb[1]);
	b = ft_atoi(rgb[2]);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		return (-1);
	return ((r << 16) | (g << 8) | b);
}
