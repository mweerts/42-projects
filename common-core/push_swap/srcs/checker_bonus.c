/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 16:36:04 by maxweert          #+#    #+#             */
/*   Updated: 2024/11/30 17:25:19 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "checker_bonus.h"

int	main(int argc, char **argv)
{
	t_data	data;
	char	*op;

	if (argc < 2)
		return (0);
	ft_parsing(&data, argc, argv);
	while (1)
	{
		op = get_next_line(0);
		if (!op)
			break ;
		if (!ft_map_op(&data, op, FALSE))
		{
			free(op);
			ft_error(&data);
		}
		free(op);
	}
	if (ft_data_issorted(&data))
		ft_putstr_fd("OK\n", 1);
	else
		ft_putstr_fd("KO\n", 1);
	ft_free_data(&data);
	return (0);
}
