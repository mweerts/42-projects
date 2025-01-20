/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:27:34 by maxweert          #+#    #+#             */
/*   Updated: 2025/01/07 18:32:59 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_print_conversion(va_list ap, t_flags flags, char specifier)
{
	if (specifier == 's')
		return (ft_print_str(ap, flags));
	if (specifier == 'c')
		return (ft_print_chr(ap, flags));
	if (specifier == 'i' || specifier == 'd')
		return (ft_print_int(ap, flags));
	if (specifier == 'u')
		return (ft_print_uint(ap, flags));
	if (specifier == 'x' || specifier == 'X')
		return (ft_print_hex(ap, flags, specifier));
	if (specifier == 'p')
		return (ft_print_ptr(ap, flags));
	if (specifier == '%')
		return (ft_print_percent());
	return (0);
}

static t_flags	ft_init_flags(void)
{
	t_flags	ret;

	ret.minus = FALSE;
	ret.plus = FALSE;
	ret.zero = FALSE;
	ret.hashtag = FALSE;
	ret.space = FALSE;
	ret.precision = -1;
	ret.width = -1;
	return (ret);
}

int	ft_printf(const char *format, ...)
{
	int		len;
	int		tmp_len;
	int		i;
	va_list	ap;
	t_flags	flags;

	i = 0;
	len = 0;
	va_start(ap, format);
	while (format[i])
	{
		if (format[i] == '%')
		{
			flags = ft_init_flags();
			i += ft_parse_flag(ap, &flags, &format[i]);
			tmp_len = ft_print_conversion(ap, flags, format[i]);
		}
		else
			tmp_len = write(1, &format[i], 1);
		if (tmp_len == -1)
			return (-1);
		len += tmp_len;
		i++;
	}
	return (va_end(ap), len);
}
