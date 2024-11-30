/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 17:04:33 by maxweert          #+#    #+#             */
/*   Updated: 2024/11/30 17:22:49 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	ft_init_data(t_data *data, int size)
{
	data->stack_a.stack = malloc(sizeof(int) * size);
	if (!(data->stack_a.stack))
		ft_error(data);
	data->stack_b.stack = malloc(sizeof(int) * size);
	if (!(data->stack_b.stack))
		ft_error(data);
	while (--size >= 0)
	{
		data->stack_a.stack[size] = -1;
		data->stack_b.stack[size] = -1;
	}
	data->stack_a.size = 0;
	data->stack_b.size = 0;
	data->op_lst = NULL;
}

int	ft_compute_rank(t_stack *stack)
{
	int	*tmp_arr;
	int	i;
	int	j;

	tmp_arr = malloc(sizeof(int) * stack->size);
	if (!tmp_arr)
		return (0);
	ft_memcpy(tmp_arr, stack->stack, sizeof(int) * stack->size);
	ft_memset(stack->stack, 0, sizeof(int) * stack->size);
	i = 0;
	while (i < stack->size)
	{
		j = 0;
		while (j < stack->size)
		{
			if (tmp_arr[j] > tmp_arr[i])
				stack->stack[j]++;
			j++;
		}
		i++;
	}
	free(tmp_arr);
	return (1);
}

t_bool	ft_check_arg(char *str)
{
	int		i;
	long	nb;
	int		sign;

	sign = 1;
	i = 0;
	nb = 0;
	if (!str[0])
		return (FALSE);
	if ((str[i] == '+' || str[i] == '-') && ft_isdigit(str[i + 1]))
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (FALSE);
		nb = (nb * 10) + (str[i] - '0');
		if ((sign == -1 && nb * -1 < INT_MIN) || (sign == 1 && nb > INT_MAX))
			return (FALSE);
		i++;
	}
	return (TRUE);
}
