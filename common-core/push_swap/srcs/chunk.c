/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 13:33:31 by maxweert          #+#    #+#             */
/*   Updated: 2024/11/28 01:54:13 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	ft_chunk_max(t_data *data, t_chunk *chunk)
{
	int	max;
	int	i;
	int	tmp;

	max = -1;
	i = 0;
	while (i < chunk->size)
	{
		if (chunk->pos == TOP_A)
			tmp = data->stack_a.stack[i];
		else if (chunk->pos == TOP_B)
			tmp = (data->stack_b.stack[i]);
		else if (chunk->pos == BOTTOM_A)
			tmp = (data->stack_a.stack[(data->stack_a.size - i) - 1]);
		else if (chunk->pos == BOTTOM_B)
			tmp = (data->stack_b.stack[(data->stack_b.size - i) - 1]);
		if (tmp > max)
			max = tmp;
		i++;
	}
	return (max);
}

int	ft_get_chunk_elem(t_data *data, t_pos pos)
{
	if (pos == TOP_A)
		return (data->stack_a.stack[0]);
	if (pos == TOP_B)
		return (data->stack_b.stack[0]);
	if (pos == BOTTOM_A)
		return (data->stack_a.stack[data->stack_a.size - 1]);
	return (data->stack_b.stack[data->stack_b.size - 1]);
}

static int	ft_get_pivot(t_data *data, t_chunk *chunk)
{
	int		ret;
	int		max;

	max = ft_chunk_max(data, chunk);
	ret = max - (chunk->size / 2);
	return (ret);
}

static void	ft_set_chunk_pos(t_pos pos, t_chunk *min, t_chunk *max)
{
	if (pos == TOP_A)
	{
		max->pos = BOTTOM_A;
		min->pos = TOP_B;
	}
	else if (pos == BOTTOM_A)
	{
		max->pos = TOP_A;
		min->pos = TOP_B;
	}
	else if (pos == TOP_B)
	{
		max->pos = TOP_A;
		min->pos = BOTTOM_B;
	}
	else if (pos == BOTTOM_B)
	{
		max->pos = TOP_A;
		min->pos = TOP_B;
	}
}

void	ft_split_chunks(t_data *data, t_chunk *chunk,
			t_chunk *min, t_chunk *max)
{
	int	pivot;
	int	i;

	pivot = ft_get_pivot(data, chunk);
	i = 0;
	max->size = 0;
	min->size = 0;
	ft_set_chunk_pos(chunk->pos, min, max);
	while (chunk->size)
	{
		if (ft_get_chunk_elem(data, chunk->pos) > pivot)
		{
			ft_move(data, chunk->pos, max->pos);
			chunk->size--;
			max->size++;
		}
		else
		{
			ft_move(data, chunk->pos, min->pos);
			chunk->size--;
			min->size++;
		}
		i++;
	}
}
