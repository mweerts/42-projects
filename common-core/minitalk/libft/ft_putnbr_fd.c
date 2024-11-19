/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:39:19 by maxweert          #+#    #+#             */
/*   Updated: 2024/10/27 16:48:34 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_recur_putnbr(long nbr, int fd, int *count)
{
	if (nbr == 0)
		return ;
	ft_recur_putnbr(nbr / 10, fd, count);
	*count += ft_putchar_fd((nbr % 10 + '0'), fd);
}

int	ft_putnbr_fd(int n, int fd)
{
	long	nbr;
	int		count;

	nbr = (long)n;
	count = 0;
	if (nbr == 0)
	{
		count += ft_putchar_fd('0', fd);
		return (count);
	}
	if (nbr < 0)
	{
		count += ft_putchar_fd('-', fd);
		nbr = -nbr;
	}
	ft_recur_putnbr(nbr, fd, &count);
	return (count);
}
