/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 23:46:21 by maxweert          #+#    #+#             */
/*   Updated: 2025/01/07 18:34:01 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_uhexlen(unsigned int uint_nbr)
{
	int	len;

	len = 0;
	if (uint_nbr == 0)
		return (1);
	while (uint_nbr != 0)
	{
		uint_nbr /= 16;
		len++;
	}
	return (len);
}

static char	*ft_set_padding(t_flags flags, char *str)
{
	int		str_len;
	int		pad_size;
	char	*padding;
	char	*tmp;

	str_len = ft_strlen(str);
	pad_size = flags.width - str_len;
	if (pad_size > 0)
	{
		padding = ft_calloc(pad_size + 1, sizeof(char));
		if (!padding)
			return (free(str), NULL);
		if (flags.zero)
			ft_memset(padding, '0', pad_size);
		else
			ft_memset(padding, ' ', pad_size);
		tmp = str;
		if (flags.minus)
			str = ft_strjoin(str, padding);
		else
			str = ft_strjoin(padding, str);
		free(tmp);
		free(padding);
	}
	return (str);
}

static char	*ft_set_number(t_flags flags, char *str,
		unsigned int nbr, char *base)
{
	int				i;
	unsigned int	tmp_nbr;
	char			*tmp;

	i = ft_strlen(str) - 1;
	tmp_nbr = nbr;
	if (nbr == 0 && flags.precision == 0 && flags.width == -1)
		str[i] = '\0';
	else if (nbr == 0 && flags.precision == 0)
		str[i] = ' ';
	while (tmp_nbr != 0)
	{
		str[i] = base[tmp_nbr % 16];
		tmp_nbr /= 16;
		i--;
	}
	tmp = str;
	if (flags.hashtag && nbr != 0 && base[10] == 'a')
		str = ft_strjoin("0x", str);
	else if (flags.hashtag && nbr != 0 && base[10] == 'A')
		str = ft_strjoin("0X", str);
	else
		str = ft_strjoin("", str);
	return (free(tmp), str);
}

static void	ft_swap_sign(char *str)
{
	int		i;
	int		j;

	i = 0;
	while (str[i] && str[i] == ' ')
		i++;
	j = i;
	while (str[i] && str[i] == '0')
		i++;
	if ((str[i] == '+' || str[i] == '-' || str[i] == ' ')
		&& ft_isdigit(str[i + 1]))
		ft_swap(&str[j], &str[i]);
}

int	ft_print_hex(va_list ap, t_flags flags, char specifier)
{
	unsigned int	nbr;
	char			*ret;
	int				ret_len;

	nbr = va_arg(ap, unsigned int);
	if (flags.zero && flags.precision != -1)
		flags.zero = FALSE;
	ret_len = ft_max(2, ft_uhexlen(nbr), flags.precision);
	ret = ft_calloc(ret_len + 1, sizeof(char));
	if (!ret)
		return (-1);
	ft_memset(ret, '0', ret_len);
	if (specifier == 'x')
		ret = ft_set_number(flags, ret, nbr, "0123456789abcdef");
	else
		ret = ft_set_number(flags, ret, nbr, "0123456789ABCDEF");
	if (!ret)
		return (-1);
	ret = ft_set_padding(flags, ret);
	ft_swap_sign(ret);
	ret_len = ft_putstr_fd(ret, 1);
	return (free(ret), ret_len);
}
