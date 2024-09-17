/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_non_printable.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 13:51:45 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/08 21:08:03 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_print_hex_number(int nbr)
{
	int	tmp;

	tmp = nbr % 16;
	if (nbr >= 16)
		ft_print_hex_number(nbr / 16);
	if (tmp < 10)
		ft_putchar(tmp + '0');
	else
		ft_putchar(tmp - 10 + 'a');
}

void	ft_put_non_printable_char(unsigned char c)
{
	ft_putchar('\\');
	if (c < 16)
		ft_putchar('0');
	ft_print_hex_number(c);
}

int	ft_char_is_printable(char c)
{
	if (c < 32 || c > 126)
		return (0);
	return (1);
}

void	ft_putstr_non_printable(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (ft_char_is_printable(str[i]))
			ft_putchar(str[i]);
		else
			ft_put_non_printable_char(str[i]);
		i++;
	}
}
