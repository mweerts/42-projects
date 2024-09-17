/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_combn.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 17:37:31 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/04 11:18:38 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_put_separator(void)
{
	ft_putchar(',');
	ft_putchar(' ');
}

void	ft_print_combn_util(int *arr, int n, int start, int index)
{
	int	i;
	int	is_last;

	i = 0;
	if (index == n)
	{
		is_last = 1;
		while (i < n)
		{
			if (arr[i] != 10 - n + i)
				is_last = 0;
			ft_putchar(arr[i] + '0');
			i++;
		}
		if (!is_last)
			ft_put_separator();
		return ;
	}
	i = start;
	while (i <= 9 && 10 - i >= n - index)
	{
		arr[index] = i;
		ft_print_combn_util(arr, n, i + 1, index + 1);
		i++;
	}
}

void	ft_print_combn(int n)
{
	int	arr[10];

	ft_print_combn_util(arr, n, 0, 0);
}
