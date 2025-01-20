/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_int.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 23:46:21 by maxweert          #+#    #+#             */
/*   Updated: 2025/01/07 18:34:01 by maxweert         ###   ########.fr       */
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
		unsigned int uint_nbr, int nbr)
{
	int		i;
	char	*tmp;

	i = ft_strlen(str) - 1;
	if (uint_nbr == 0 && flags.precision == 0 && flags.width == -1)
		str[i] = '\0';
	else if (uint_nbr == 0 && flags.precision == 0)
		str[i] = ' ';
	while (uint_nbr != 0)
	{
		str[i] = (uint_nbr % 10) + '0';
		uint_nbr /= 10;
		i--;
	}
	tmp = str;
	if (nbr < 0)
		str = ft_strjoin("-", str);
	else if (flags.plus)
		str = ft_strjoin("+", str);
	else if (flags.space)
		str = ft_strjoin(" ", str);
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

int	ft_print_int(va_list ap, t_flags flags)
{
	int				nbr;
	unsigned int	uint_nbr;
	char			*ret;
	int				ret_len;

	nbr = va_arg(ap, int);
	if (flags.zero && flags.precision != -1)
		flags.zero = FALSE;
	if (nbr < 0)
		uint_nbr = 0 - (unsigned)nbr;
	else
		uint_nbr = (unsigned)nbr;
	ret_len = ft_max(2, ft_uintlen(uint_nbr), flags.precision);
	ret = ft_calloc(ret_len + 1, sizeof(char));
	if (!ret)
		return (-1);
	ft_memset(ret, '0', ret_len);
	ret = ft_set_number(flags, ret, uint_nbr, nbr);
	if (!ret)
		return (-1);
	ret = ft_set_padding(flags, ret);
	ft_swap_sign(ret);
	ret_len = ft_putstr_fd(ret, 1);
	return (free(ret), ret_len);
}
