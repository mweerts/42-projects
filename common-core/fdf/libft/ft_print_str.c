/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_str.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 20:06:22 by maxweert          #+#    #+#             */
/*   Updated: 2025/01/07 18:34:01 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_get_ret_string(t_flags flags, char *str)
{
	int		str_len;
	int		ret_len;
	char	*ret;

	str_len = ft_strlen(str);
	if (flags.precision != -1 && flags.precision < str_len)
	{
		str[flags.precision] = '\0';
		str_len = flags.precision;
	}
	ret_len = str_len;
	if (ret_len < flags.width)
		ret_len = flags.width;
	ret = ft_calloc(ret_len + 1, sizeof(char));
	if (!ret)
		return (NULL);
	ft_memset(ret, ' ', ret_len);
	if (flags.minus)
		ft_memcpy(ret, str, str_len);
	else
		ft_memcpy(&ret[ret_len - str_len], str, str_len);
	return (ret);
}

int	ft_print_str(va_list ap, t_flags flags)
{
	char	*str;
	char	*ret;
	int		ret_len;

	str = va_arg(ap, char *);
	if (!str)
	{
		if (flags.precision > 0 && flags.precision < 6)
			str = ft_strdup("");
		else
			str = ft_strdup("(null)");
	}
	else
		str = ft_strdup(str);
	if (!str)
		return (-1);
	ret = ft_get_ret_string(flags, str);
	if (!ret)
		return (free(str), -1);
	ret_len = ft_putstr_fd(ret, 1);
	return (free(ret), free(str), ret_len);
}
