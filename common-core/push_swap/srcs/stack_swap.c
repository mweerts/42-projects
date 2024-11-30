/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_swap.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 17:10:28 by maxweert          #+#    #+#             */
/*   Updated: 2024/11/25 18:32:20 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	ft_stack_swap(t_stack *stack)
{
	int	tmp;

	tmp = 0;
	if (stack->size < 2)
		return ;
	tmp = stack->stack[0];
	stack->stack[0] = stack->stack[1];
	stack->stack[1] = tmp;
}

void	ft_sa(t_data *data)
{
	ft_stack_swap(&data->stack_a);
}

void	ft_sb(t_data *data)
{
	ft_stack_swap(&data->stack_b);
}

void	ft_ss(t_data *data)
{
	ft_sa(data);
	ft_sb(data);
}
