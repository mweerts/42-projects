/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_ptr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 00:10:25 by maxweert          #+#    #+#             */
/*   Updated: 2025/01/07 18:34:01 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_uhexlen(unsigned long long uhex_nbr)
{
	int	len;

	len = 0;
	if (uhex_nbr == 0)
		return (1);
	while (uhex_nbr != 0)
	{
		uhex_nbr /= 16;
		len++;
	}
	return (len);
}

static void	ft_set_ret_string(t_flags flags,
		unsigned long long nbr, char *ret, char *base)
{
	int	i;
	int	nbr_len;

	nbr_len = ft_uhexlen(nbr) + 2;
	if (flags.minus)
		i = nbr_len - 1;
	else
		i = ft_strlen(ret) - 1;
	while (nbr != 0)
	{
		ret[i--] = base[nbr % 16];
		nbr /= 16;
	}
	while (i >= 0 && flags.zero)
	{
		ret[i--] = '0';
		nbr_len++;
	}
	ret[i] = 'x';
	ret[i - 1] = '0';
}

int	ft_print_ptr(va_list ap, t_flags flags)
{
	unsigned long long	nbr;
	char				*ret;
	int					ret_len;
	int					str_len;

	nbr = va_arg(ap, unsigned long long);
	str_len = ft_uhexlen(nbr) + 2;
	if (flags.precision != -1 && flags.zero)
		flags.zero = FALSE;
	if (nbr == 0)
		str_len = 5;
	ret_len = ft_max(3, str_len, flags.width, flags.precision);
	ret = ft_calloc(ret_len + 1, sizeof(char));
	if (!ret)
		return (-1);
	ft_memset(ret, ' ', ret_len);
	if (nbr == 0 && flags.minus)
		ft_memmove(&ret[0], "(nil)", 5);
	else if (nbr == 0 && !flags.minus)
		ft_memmove(&ret[ret_len - 5], "(nil)", 5);
	else
		ft_set_ret_string(flags, nbr, ret, "0123456789abcdef");
	ret_len = ft_putstr_fd(ret, 1);
	return (free(ret), ret_len);
}
