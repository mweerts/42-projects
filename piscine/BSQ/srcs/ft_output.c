/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_output.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 20:36:44 by apierret          #+#    #+#             */
/*   Updated: 2024/09/25 10:44:41 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_bsq.h"

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putstr(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		ft_putchar(str[i++]);
}

void	ft_putchar_err(char c)
{
	write(2, &c, 1);
}

void	ft_putstr_err(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		ft_putchar_err(str[i++]);
}
