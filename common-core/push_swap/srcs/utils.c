/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 17:26:30 by maxweert          #+#    #+#             */
/*   Updated: 2024/11/30 17:04:48 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	ft_error(t_data *data)
{
	ft_free_data(data);
	ft_putstr_fd("Error\n", 2);
	exit(1);
}

void	ft_free_data(t_data *data)
{
	if (data->stack_a.stack)
		free(data->stack_a.stack);
	if (data->stack_b.stack)
		free(data->stack_b.stack);
	if (data->op_lst)
		ft_lstclear(&data->op_lst, &free);
}

static int	ft_do_op(t_data *data, char *op)
{
	if (ft_strncmp(op, "pa\n", 3) == 0)
		ft_pa(data);
	else if (ft_strncmp(op, "pb\n", 3) == 0)
		ft_pb(data);
	else if (ft_strncmp(op, "sa\n", 3) == 0)
		ft_sa(data);
	else if (ft_strncmp(op, "sb\n", 3) == 0)
		ft_sb(data);
	else if (ft_strncmp(op, "ss\n", 3) == 0)
		ft_ss(data);
	else if (ft_strncmp(op, "ra\n", 3) == 0)
		ft_ra(data);
	else if (ft_strncmp(op, "rb\n", 3) == 0)
		ft_rb(data);
	else if (ft_strncmp(op, "rr\n", 3) == 0)
		ft_rr(data);
	else if (ft_strncmp(op, "rra\n", 4) == 0)
		ft_rra(data);
	else if (ft_strncmp(op, "rrb\n", 4) == 0)
		ft_rrb(data);
	else if (ft_strncmp(op, "rrr\n", 4) == 0)
		ft_rrr(data);
	else
		return (0);
	return (1);
}

int	ft_map_op(t_data *data, char *op, t_bool print)
{
	if (!ft_do_op(data, op))
		return (0);
	if (print)
		ft_lstadd_back(&data->op_lst, ft_lstnew(ft_strdup(op)));
	return (1);
}

t_bool	ft_data_issorted(t_data *data)
{
	int	i;

	i = 0;
	if (data->stack_b.size != 0)
		return (FALSE);
	while (i < data->stack_a.size - 1)
	{
		if (data->stack_a.stack[i] >= data->stack_a.stack[i + 1])
			return (FALSE);
		i++;
	}
	return (TRUE);
}
