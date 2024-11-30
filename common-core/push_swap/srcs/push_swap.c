/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 16:56:23 by maxweert          #+#    #+#             */
/*   Updated: 2024/11/30 17:27:35 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	ft_print_op_lst(t_data *data)
{
	t_list	*head;

	head = data->op_lst;
	while (head)
	{
		ft_putstr_fd(head->content, 1);
		head = head->next;
	}
}

int	main(int argc, char **argv)
{
	t_data	data;
	t_chunk	chunk;

	if (argc < 2)
		return (0);
	data.op_lst = NULL;
	ft_parsing(&data, argc, argv);
	chunk.size = data.stack_a.size;
	chunk.pos = TOP_A;
	ft_quicksort(&data, &chunk);
	ft_remove_neutrals(&data.op_lst);
	ft_print_op_lst(&data);
	ft_free_data(&data);
	return (0);
}
