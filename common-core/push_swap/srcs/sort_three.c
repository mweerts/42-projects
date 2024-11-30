/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_three.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 00:21:44 by maxweert          #+#    #+#             */
/*   Updated: 2024/11/28 16:32:27 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	ft_sort_three_from_topa(t_data *data, t_chunk *chunk)
{
	int	max;

	max = ft_chunk_max(data, chunk);
	if (chunk->size == data->stack_a.size && max == data->stack_a.stack[0])
		ft_map_op(data, "ra\n", TRUE);
	else if (chunk->size == data->stack_a.size && max == data->stack_a.stack[1])
		ft_map_op(data, "rra\n", TRUE);
	else if (max == data->stack_a.stack[0])
	{
		ft_map_op(data, "sa\n", TRUE);
		ft_map_op(data, "ra\n", TRUE);
		ft_map_op(data, "sa\n", TRUE);
		ft_map_op(data, "rra\n", TRUE);
	}
	else if (max == data->stack_a.stack[1])
	{
		ft_map_op(data, "ra\n", TRUE);
		ft_map_op(data, "sa\n", TRUE);
		ft_map_op(data, "rra\n", TRUE);
	}
	ft_sort_two(data, chunk);
}

static void	ft_sort_three_from_topb(t_data *data, t_chunk *chunk)
{
	int	max;

	max = ft_chunk_max(data, chunk);
	ft_map_op(data, "pa\n", TRUE);
	if (data->stack_b.stack[0] == max)
	{
		ft_map_op(data, "pa\n", TRUE);
		ft_map_op(data, "sa\n", TRUE);
	}
	else if (data->stack_b.stack[1] == max)
	{
		ft_map_op(data, "sb\n", TRUE);
		ft_map_op(data, "pa\n", TRUE);
		ft_map_op(data, "sa\n", TRUE);
	}
	else
		ft_map_op(data, "pa\n", TRUE);
	ft_map_op(data, "pa\n", TRUE);
	chunk->pos = TOP_A;
	ft_sort_two(data, chunk);
}

static void	ft_sort_three_from_bottoma(t_data *data, t_chunk *chunk)
{
	int	max;

	max = ft_chunk_max(data, chunk);
	ft_map_op(data, "rra\n", TRUE);
	ft_map_op(data, "rra\n", TRUE);
	if (data->stack_a.stack[data->stack_a.size - 1] == max)
	{
		ft_map_op(data, "pb\n", TRUE);
		ft_map_op(data, "rra\n", TRUE);
		ft_map_op(data, "sa\n", TRUE);
		ft_map_op(data, "pa\n", TRUE);
	}
	else if (data->stack_a.stack[0] == max)
	{
		ft_map_op(data, "sa\n", TRUE);
		ft_map_op(data, "rra\n", TRUE);
	}
	else if (data->stack_a.stack[1] == max)
		ft_map_op(data, "rra\n", TRUE);
	chunk->pos = TOP_A;
	ft_sort_two(data, chunk);
}

static void	ft_sort_three_from_bottomb(t_data *data, t_chunk *chunk)
{
	int	max;

	max = ft_chunk_max(data, chunk);
	ft_map_op(data, "rrb\n", TRUE);
	ft_map_op(data, "rrb\n", TRUE);
	if (data->stack_b.stack[data->stack_b.size - 1] == max)
	{
		ft_map_op(data, "rrb\n", TRUE);
		ft_map_op(data, "pa\n", TRUE);
	}
	else if (data->stack_b.stack[0] == max)
	{
		ft_map_op(data, "pa\n", TRUE);
		ft_map_op(data, "rrb\n", TRUE);
	}
	else if (data->stack_b.stack[1] == max)
	{
		ft_map_op(data, "sb\n", TRUE);
		ft_map_op(data, "pa\n", TRUE);
		ft_map_op(data, "rrb\n", TRUE);
	}
	ft_map_op(data, "pa\n", TRUE);
	ft_map_op(data, "pa\n", TRUE);
	chunk->pos = TOP_A;
	ft_sort_two(data, chunk);
}

void	ft_sort_three(t_data *data, t_chunk *chunk)
{
	if (chunk->pos == TOP_A)
		ft_sort_three_from_topa(data, chunk);
	else if (chunk->pos == TOP_B)
		ft_sort_three_from_topb(data, chunk);
	else if (chunk->pos == BOTTOM_A)
		ft_sort_three_from_bottoma(data, chunk);
	else if (chunk->pos == BOTTOM_B)
		ft_sort_three_from_bottomb(data, chunk);
}
