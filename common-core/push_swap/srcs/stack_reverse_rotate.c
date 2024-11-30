/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_reverse_rotate.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 18:59:59 by maxweert          #+#    #+#             */
/*   Updated: 2024/11/28 17:39:46 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	ft_stack_reverse_rotate(t_stack *stack)
{
	int	tmp;

	if (stack->size < 2)
		return ;
	tmp = stack->stack[stack->size - 1];
	ft_memmove(stack->stack + 1, stack->stack, sizeof(int) * (stack->size - 1));
	stack->stack[0] = tmp;
}

void	ft_rra(t_data *data)
{
	ft_stack_reverse_rotate(&data->stack_a);
}

void	ft_rrb(t_data *data)
{
	ft_stack_reverse_rotate(&data->stack_b);
}

void	ft_rrr(t_data *data)
{
	ft_rra(data);
	ft_rrb(data);
}
