/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_rotate.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 18:53:31 by maxweert          #+#    #+#             */
/*   Updated: 2024/11/28 17:39:53 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	ft_stack_rotate(t_stack *stack)
{
	int	tmp;

	if (stack->size < 2)
		return ;
	tmp = stack->stack[0];
	ft_memmove(stack->stack, stack->stack + 1, sizeof(int) * (stack->size - 1));
	stack->stack[stack->size - 1] = tmp;
}

void	ft_ra(t_data *data)
{
	ft_stack_rotate(&data->stack_a);
}

void	ft_rb(t_data *data)
{
	ft_stack_rotate(&data->stack_b);
}

void	ft_rr(t_data *data)
{
	ft_ra(data);
	ft_rb(data);
}
