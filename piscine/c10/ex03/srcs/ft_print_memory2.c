/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_memory2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 14:28:29 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/24 11:42:48 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_hexdump.h"

void	ft_print_hex_number(unsigned long nbr, int str_size)
{
	int	tmp;

	tmp = nbr % 16;
	if (nbr >= 16)
		ft_print_hex_number(nbr / 16, str_size - 1);
	else
	{
		while (str_size > 1)
		{
			ft_putchar('0');
			str_size--;
		}
	}
	if (tmp < 10)
		ft_putchar(tmp + '0');
	else
		ft_putchar(tmp - 10 + 'a');
}

void	ft_print_content(unsigned char *addr, unsigned int size)
{
	unsigned int	i;

	i = 0;
	while (i < size && i < 16)
	{
		if (addr[i] < 32 || addr[i] > 126)
			ft_putchar('.');
		else
			ft_putchar(addr[i]);
		i++;
	}
}

void	ft_print_memory_flag(void *addr, unsigned int size, int line)
{
	if (size == 0)
	{
		ft_print_hex_number(line, 8);
		ft_putchar('\n');
		return ;
	}
	ft_print_hex_number(line, 8);
	ft_putchar(' ');
	ft_putchar(' ');
	ft_print_content_hex((unsigned char *)addr, size);
	ft_putchar('|');
	ft_print_content((unsigned char *)addr, size);
	ft_putchar('|');
	ft_putchar('\n');
}

void	ft_print_memory_noflag(void *addr, unsigned int size, int line)
{
	if (size == 0)
	{
		ft_print_hex_number(line, 7);
		ft_putchar('\n');
		return ;
	}
	ft_print_hex_number(line, 7);
	ft_putchar(' ');
	ft_print_content_hex_noflag((unsigned char *)addr, size);
	ft_putchar('\n');
}
