/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_operations.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 13:36:04 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/15 14:47:13 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_do_op.h"

void	ft_addition(int a, int b)
{
	ft_putnbr(a + b);
	ft_putstr("\n");
}

void	ft_soustraction(int a, int b)
{
	ft_putnbr(a - b);
	ft_putstr("\n");
}

void	ft_multiplication(int a, int b)
{
	ft_putnbr(a * b);
	ft_putstr("\n");
}

void	ft_division(int a, int b)
{
	if (b == 0)
		ft_putstr("Stop : division by zero\n");
	else
	{
		ft_putnbr(a / b);
		ft_putstr("\n");
	}
}

void	ft_modulo(int a, int b)
{
	if (b == 0)
		ft_putstr("Stop : modulo by zero\n");
	else
	{
		ft_putnbr(a % b);
		ft_putstr("\n");
	}
}
