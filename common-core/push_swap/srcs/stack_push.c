/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_push.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 16:54:36 by maxweert          #+#    #+#             */
/*   Updated: 2024/11/25 18:32:48 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	ft_stack_push(t_stack *src, t_stack *dst)
{
	int	tmp;

	if (src->size < 1)
		return ;
	tmp = src->stack[0];
	src->size--;
	ft_memmove(src->stack, src->stack + 1, sizeof(int) * src->size);
	src->stack[src->size] = -1;
	ft_memmove(dst->stack + 1, dst->stack, sizeof(int) * dst->size);
	dst->size++;
	dst->stack[0] = tmp;
}

void	ft_pa(t_data *data)
{
	ft_stack_push(&data->stack_b, &data->stack_a);
}

void	ft_pb(t_data *data)
{
	ft_stack_push(&data->stack_a, &data->stack_b);
}
