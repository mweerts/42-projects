/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_uint.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 23:51:46 by maxweert          #+#    #+#             */
/*   Updated: 2025/01/07 18:33:07 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_uintlen(unsigned int uint_nbr)
{
	int	len;

	len = 0;
	if (uint_nbr == 0)
		return (1);
	while (uint_nbr != 0)
	{
		uint_nbr /= 10;
		len++;
	}
	return (len);
}

static void	ft_set_ret_string(t_flags flags, unsigned int nbr, char *ret)
{
	int	i;
	int	nbr_len;

	nbr_len = ft_uintlen(nbr);
	if (flags.minus && flags.precision > nbr_len)
		i = flags.precision - 1;
	else if (flags.minus)
		i = nbr_len - 1;
	else
		i = ft_strlen(ret) - 1;
	if (nbr == 0 && flags.precision != 0)
		ret[i--] = '0';
	while (nbr != 0)
	{
		ret[i--] = (nbr % 10) + '0';
		nbr /= 10;
	}
	while (i >= 0 && ((flags.zero && flags.precision != 0)
			|| flags.precision - nbr_len > 0))
	{
		ret[i--] = '0';
		nbr_len++;
	}
}

int	ft_print_uint(va_list ap, t_flags flags)
{
	long	nbr;
	char	*ret;
	int		ret_len;

	nbr = va_arg(ap, unsigned int);
	if (nbr == 0 && flags.width == -1 && flags.precision == 0)
		return (write(1, "", 0));
	if (flags.precision != -1 && flags.zero)
		flags.zero = FALSE;
	ret_len = ft_max(3, ft_uintlen(nbr), flags.width, flags.precision);
	ret = ft_calloc(ret_len + 1, sizeof(char));
	if (!ret)
		return (-1);
	ft_memset(ret, ' ', ret_len);
	ft_set_ret_string(flags, nbr, ret);
	ret_len = ft_putstr_fd(ret, 1);
	return (free(ret), ret_len);
}
