/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_memory.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 14:28:29 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/09 12:38:48 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

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

void	ft_print_content_hex(unsigned char *addr, unsigned int size)
{
	unsigned int	i;
	int				chr_count;

	i = 0;
	chr_count = 0;
	while (i < size && i < 16)
	{
		ft_print_hex_number((unsigned long)addr[i], 2);
		chr_count += 2;
		if (i % 2 == 1)
		{
			ft_putchar(' ');
			chr_count++;
		}
		i++;
	}
	while (chr_count++ < 40)
		ft_putchar(' ');
}

void	*ft_print_memory(void *addr, unsigned int size)
{
	unsigned int	i;
	unsigned int	j;
	unsigned char	*tmp;

	i = 0;
	tmp = (unsigned char *) addr;
	if (size == 0)
		return (addr);
	while (i < size)
	{
		j = 0;
		ft_print_hex_number((unsigned long)addr + i, 16);
		ft_putchar(':');
		ft_putchar(' ');
		ft_print_content_hex((unsigned char *)addr + i, size - i);
		ft_print_content((unsigned char *)addr + i, size - i);
		ft_putchar('\n');
		i += 16;
	}
	return (addr);
}
