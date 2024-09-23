/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_convert_base.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 11:46:57 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/10 12:49:02 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

int		ft_strlen(char *str);
int		ft_check_base(char *base);
int		ft_char_is_in_base(char c, char *base);
int		ft_char_index(char c, char *base);
char	*ft_strcat_char(char *dest, char c);

void	ft_itoa_base_util(long nbr, char *dest, char *base, int base_len)
{
	if (nbr > base_len - 1)
		ft_itoa_base_util(nbr / base_len, dest, base, base_len);
	ft_strcat_char(dest, base[nbr % base_len]);
}

char	*ft_itoa_base(int nbr, char *base_to)
{
	long	long_nbr;
	int		str_size;
	char	*ret;

	long_nbr = (long)nbr;
	str_size = 0;
	if (long_nbr < 0)
		str_size++;
	while (long_nbr != 0)
	{
		str_size++;
		long_nbr = long_nbr / ft_strlen(base_to);
	}
	ret = (char *)malloc((str_size + 1) * sizeof(char));
	*ret = 0;
	if (!ret)
		return (0);
	long_nbr = (long)nbr;
	if (long_nbr < 0)
	{
		ft_strcat_char(ret, '-');
		long_nbr = -long_nbr;
	}
	ft_itoa_base_util(long_nbr, ret, base_to, ft_strlen(base_to));
	return (ret);
}

int	ft_atoi_base(char *nbr, char *base_from)
{
	int		i;
	long	res;
	int		sign;

	i = 0;
	res = 0;
	sign = 1;
	while (nbr[i] == ' ' || nbr[i] == '\t' || nbr[i] == '\n'
		|| nbr[i] == '\v' || nbr[i] == '\f' || nbr[i] == '\r')
		i++;
	while (nbr[i] == '-' || nbr[i] == '+')
	{
		if (nbr[i] == '-')
			sign = -sign;
		i++;
	}
	while (ft_char_is_in_base(nbr[i], base_from))
	{
		res = (res * ft_strlen(base_from)) + ft_char_index(nbr[i], base_from);
		i++;
	}
	return ((int)res * sign);
}

char	*ft_convert_base(char *nbr, char *base_from, char *base_to)
{
	if (!ft_check_base(base_from) || !ft_check_base(base_to))
		return (0);
	return (ft_itoa_base(ft_atoi_base(nbr, base_from), base_to));
}

// #include <stdio.h>

// int main()
// {
// 	printf("%s", ft_convert_base("1234", "0123456789", "0123456789"));
// }