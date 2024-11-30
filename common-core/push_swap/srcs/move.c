/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 14:45:43 by maxweert          #+#    #+#             */
/*   Updated: 2024/11/27 15:25:37 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	ft_move_from_top_a(t_data *data, t_pos to)
{
	if (to == TOP_B)
		ft_map_op(data, "pb\n", TRUE);
	else if (to == BOTTOM_A)
		ft_map_op(data, "ra\n", TRUE);
	else if (to == BOTTOM_B)
	{
		ft_map_op(data, "pb\n", TRUE);
		ft_map_op(data, "rb\n", TRUE);
	}
	return ;
}

static void	ft_move_from_bottom_a(t_data *data, t_pos to)
{
	if (to == TOP_A)
		ft_map_op(data, "rra\n", TRUE);
	else if (to == TOP_B)
	{
		ft_map_op(data, "rra\n", TRUE);
		ft_map_op(data, "pb\n", TRUE);
	}
	else if (to == BOTTOM_B)
	{
		ft_map_op(data, "rra\n", TRUE);
		ft_map_op(data, "pb\n", TRUE);
		ft_map_op(data, "rb\n", TRUE);
	}
	return ;
}

static void	ft_move_from_top_b(t_data *data, t_pos to)
{
	if (to == TOP_A)
		ft_map_op(data, "pa\n", TRUE);
	else if (to == BOTTOM_B)
		ft_map_op(data, "rb\n", TRUE);
	else if (to == BOTTOM_A)
	{
		ft_map_op(data, "pa\n", TRUE);
		ft_map_op(data, "ra\n", TRUE);
	}
	return ;
}

static void	ft_move_from_bottom_b(t_data *data, t_pos to)
{
	if (to == TOP_B)
		ft_map_op(data, "rrb\n", TRUE);
	else if (to == TOP_A)
	{
		ft_map_op(data, "rrb\n", TRUE);
		ft_map_op(data, "pa\n", TRUE);
	}
	else if (to == BOTTOM_A)
	{
		ft_map_op(data, "rrb\n", TRUE);
		ft_map_op(data, "pa\n", TRUE);
		ft_map_op(data, "ra\n", TRUE);
	}
	return ;
}

void	ft_move(t_data *data, t_pos from, t_pos to)
{
	if (from == TOP_A)
		ft_move_from_top_a(data, to);
	else if (from == BOTTOM_A)
		ft_move_from_bottom_a(data, to);
	else if (from == TOP_B)
		ft_move_from_top_b(data, to);
	else if (from == BOTTOM_B)
		ft_move_from_bottom_b(data, to);
}
