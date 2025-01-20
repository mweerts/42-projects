/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_chr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 23:35:32 by maxweert          #+#    #+#             */
/*   Updated: 2025/01/07 18:34:01 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_print_chr(va_list ap, t_flags flags)
{
	char	c;
	char	*ret;
	int		len;

	c = va_arg(ap, int);
	len = 1;
	if (flags.width > 1)
		len = flags.width;
	ret = ft_calloc(len + 1, sizeof(char));
	if (!ret)
		return (-1);
	ft_memset(ret, ' ', len);
	if (flags.minus)
		ret[0] = c;
	else
		ret[len - 1] = c;
	len = write(1, ret, len);
	return (free(ret), len);
}
