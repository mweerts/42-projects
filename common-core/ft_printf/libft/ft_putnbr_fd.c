/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:39:19 by maxweert          #+#    #+#             */
/*   Updated: 2024/10/10 14:46:20 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_recur_putnbr(long nbr, int fd)
{
	if (nbr == 0)
		return ;
	ft_recur_putnbr(nbr / 10, fd);
	ft_putchar_fd((nbr % 10 + '0'), fd);
}

void	ft_putnbr_fd(int n, int fd)
{
	long	nbr;

	nbr = (long)n;
	if (nbr == 0)
	{
		ft_putchar_fd('0', fd);
		return ;
	}
	if (nbr < 0)
	{
		ft_putchar_fd('-', fd);
		nbr = -nbr;
	}
	ft_recur_putnbr(nbr, fd);
}
