/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 19:00:08 by maxweert          #+#    #+#             */
/*   Updated: 2025/01/07 18:34:01 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_parse_chr(const char *s_flags, t_flags *flags)
{
	if (*s_flags == '-')
		flags->minus = TRUE;
	if (*s_flags == '+')
		flags->plus = TRUE;
	if (*s_flags == ' ')
		flags->space = TRUE;
	if (*s_flags == '0')
		flags->zero = TRUE;
	if (*s_flags == '#')
		flags->hashtag = TRUE;
	return (1);
}

static int	ft_parse_number(va_list ap, const char *s_flags, int *flags_value)
{
	int	i;

	i = 0;
	if (s_flags[i] == '*')
	{
		*flags_value = (int)va_arg(ap, int);
		i++;
	}
	else if (ft_isdigit(s_flags[i]))
	{
		*flags_value = ft_atoi(&s_flags[i]);
		while (ft_isdigit(s_flags[i]))
			i++;
	}
	return (i);
}

int	ft_parse_flag(va_list ap, t_flags *flags, const char *s_flags)
{
	int		i;

	i = 1;
	while (s_flags[i] && s_flags[i] != '.' && s_flags[i] != '%'
		&& (!ft_isalnum(s_flags[i]) || ft_isinset(s_flags[i], "-+0# ")))
		i += ft_parse_chr(&s_flags[i], flags);
	if (ft_isdigit(s_flags[i]))
		i += ft_parse_number(ap, &s_flags[i], &flags->width);
	if (s_flags[i] == '.')
	{
		i++;
		if (s_flags[i] == '*' || ft_isdigit(s_flags[i]))
			i += ft_parse_number(ap, &s_flags[i], &flags->precision);
		else
			flags->precision = 0;
	}
	return (i);
}
