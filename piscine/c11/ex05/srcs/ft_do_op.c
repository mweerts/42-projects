/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_do_op.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 13:30:20 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/15 14:47:14 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_do_op.h"

int	ft_isvalid_op(char *str)
{
	if (ft_strlen(str) != 1)
		return (0);
	if (!(str[0] == '+' || str[0] == '-' || str[0] == '*'
			|| str[0] == '/' || str[0] == '%'))
		return (0);
	return (1);
}

void	ft_set_ftab(t_function *ftab[5])
{
	ftab[0] = ft_addition;
	ftab[1] = ft_soustraction;
	ftab[2] = ft_multiplication;
	ftab[3] = ft_division;
	ftab[4] = ft_modulo;
}

int	main(int argc, char **argv)
{
	char		*ops;
	int			i;
	t_function	*ftab[5];

	if (argc != 4)
		return (0);
	if (!ft_isvalid_op(argv[2]))
	{
		ft_putstr("0\n");
		return (0);
	}
	i = 0;
	ops = "+-*/%";
	ft_set_ftab(ftab);
	while (i < ft_strlen(ops))
	{
		if (argv[2][0] == ops[i])
			ftab[i](ft_atoi(argv[1]), ft_atoi(argv[3]));
		i++;
	}
	return (0);
}
