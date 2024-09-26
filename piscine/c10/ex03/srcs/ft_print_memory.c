/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_memory.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-09-24 07:45:30 by maxweert          #+#    #+#             */
/*   Updated: 2024-09-24 07:45:30 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_hexdump.h"

void	ft_print_memory(void *addr, unsigned int size, int line, int flag)
{
	if (flag)
		ft_print_memory_flag(addr, size, line);
	else
		ft_print_memory_noflag(addr, size, line);
}

void	ft_print_content_hex(unsigned char *addr, unsigned int size)
{
	unsigned int	i;
	int				chr_count;

	i = 0;
	chr_count = 0;
	while (i < size && i < 16)
	{
		ft_print_hex_number((unsigned long)addr[i], 2);
		ft_putchar(' ');
		chr_count += 3;
		if (i == 7)
		{
			ft_putchar(' ');
			chr_count++;
		}
		i++;
	}
	while (chr_count++ < 50)
		ft_putchar(' ');
}

void	ft_print_content_hex_noflag(unsigned char *addr, unsigned int size)
{
	unsigned int	i;
	int				chr_count;

	i = 0;
	chr_count = 0;
	while (i < size && i < 16)
	{
		ft_print_hex_number((unsigned long)addr[i], 2);
		chr_count += 2;
		if (i + 1 < size)
		{
			ft_putchar(' ');
			chr_count++;
		}
		i++;
	}
	while (chr_count++ < 47)
		ft_putchar(' ');
}
