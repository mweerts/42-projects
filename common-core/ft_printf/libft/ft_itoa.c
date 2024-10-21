/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:07:08 by maxweert          #+#    #+#             */
/*   Updated: 2024/10/10 14:23:37 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_nbrlen(int n)
{
	size_t	len;

	len = 0;
	if (n == 0)
		return (1);
	if (n < 0)
		len++;
	while (n != 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char	*ret;
	size_t	len;
	long	nbr;

	len = ft_nbrlen(n);
	ret = ft_calloc(len + 1, sizeof(char));
	nbr = (long)n;
	if (!ret)
		return (NULL);
	if (nbr < 0)
	{
		nbr = -nbr;
		ret[0] = '-';
	}
	if (nbr == 0)
		ret[0] = '0';
	len--;
	while (nbr != 0)
	{
		ret[len] = (nbr % 10) + '0';
		nbr /= 10;
		len--;
	}
	return (ret);
}
