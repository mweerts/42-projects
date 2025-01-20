/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_conversions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 20:07:52 by maxweert          #+#    #+#             */
/*   Updated: 2025/01/07 18:34:01 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_print_conversion(va_list ap, t_flag flags, char specifier)
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
