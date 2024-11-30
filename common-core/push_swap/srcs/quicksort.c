/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quicksort.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 14:22:37 by maxweert          #+#    #+#             */
/*   Updated: 2024/11/28 17:05:20 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	ft_sort_case_five(t_data *data, t_chunk *chunk)
{
	while (data->stack_a.size > 3)
	{
		if (data->stack_a.stack[0] == 0 || data->stack_a.stack[0] == 1)
			ft_map_op(data, "pb\n", TRUE);
		else
			ft_map_op(data, "ra\n", TRUE);
	}
	chunk->size = 3;
	ft_sort_three(data, chunk);
	if (data->stack_b.stack[0] < data->stack_b.stack[1])
		ft_map_op(data, "sb\n", TRUE);
	ft_map_op(data, "pa\n", TRUE);
	ft_map_op(data, "pa\n", TRUE);
}

void	ft_sort_two(t_data *data, t_chunk *chunk)
{
	if (chunk->pos != TOP_A)
	{
		ft_move(data, chunk->pos, TOP_A);
		ft_move(data, chunk->pos, TOP_A);
	}
	if (data->stack_a.stack[0] > data->stack_a.stack[1])
		ft_map_op(data, "sa\n", TRUE);
}

static void	ft_opti_chunk_pos(t_data *data, t_chunk *chunk)
{
	if (chunk->pos == BOTTOM_B && data->stack_b.size == chunk->size)
		chunk->pos = TOP_B;
	if (chunk->pos == BOTTOM_A && data->stack_a.size == chunk->size)
		chunk->pos = TOP_A;
}

void	ft_quicksort(t_data *data, t_chunk *chunk)
{
	t_chunk	max;
	t_chunk	min;

	if (ft_data_issorted(data))
		return ;
	if (chunk->size == 5 && chunk->size == data->stack_a.size
		&& chunk->pos == TOP_A)
	{
		ft_sort_case_five(data, chunk);
		return ;
	}
	if (chunk->size <= 3)
	{
		if (chunk->size == 1)
			ft_move(data, chunk->pos, TOP_A);
		if (chunk->size == 2)
			ft_sort_two(data, chunk);
		else if (chunk->size == 3)
			ft_sort_three(data, chunk);
		return ;
	}
	ft_opti_chunk_pos(data, chunk);
	ft_split_chunks(data, chunk, &min, &max);
	ft_quicksort(data, &max);
	ft_quicksort(data, &min);
}
