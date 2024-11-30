/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 16:57:09 by maxweert          #+#    #+#             */
/*   Updated: 2024/11/30 17:29:56 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	ft_tab_size(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

static void	ft_free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

static void	ft_parse_args(int size, char **tab, t_data *data)
{
	int	i;
	int	j;

	if (size <= 0)
		ft_error(data);
	data->stack_a.size = size;
	i = 0;
	while (i < size)
	{
		if (!ft_check_arg(tab[i]))
			ft_error(data);
		j = 0;
		while (j < i)
		{
			if (data->stack_a.stack[j] == ft_atoi(tab[i]))
				ft_error(data);
			j++;
		}
		data->stack_a.stack[i] = ft_atoi(tab[i]);
		i++;
	}
	ft_compute_rank(&data->stack_a);
}

static void	ft_parse_malloc_args(int size, char **tab, t_data *data)
{
	int	i;
	int	j;

	data->stack_a.size = size;
	i = -1;
	while (++i < size)
	{
		if (!ft_check_arg(tab[i]))
		{
			ft_free_tab(tab);
			ft_error(data);
		}
		j = 0;
		while (j < i)
		{
			if (data->stack_a.stack[j] == ft_atoi(tab[i]))
			{
				ft_free_tab(tab);
				ft_error(data);
			}
			j++;
		}
		data->stack_a.stack[i] = ft_atoi(tab[i]);
	}
	ft_compute_rank(&data->stack_a);
}

void	ft_parsing(t_data *data, int argc, char **argv)
{
	char	**tab;
	int		size;

	size = 0;
	if (argc == 2)
	{
		tab = ft_split(argv[1], ' ');
		if (!tab)
			ft_error(data);
		size = ft_tab_size(tab);
		ft_free_tab(tab);
	}
	else
		size = argc - 1;
	ft_init_data(data, size);
	if (argc == 2)
	{
		tab = ft_split(argv[1], ' ');
		if (!tab || !tab[0])
			ft_error(data);
		ft_parse_malloc_args(size, tab, data);
		ft_free_tab(tab);
	}
	else
		ft_parse_args(size, &argv[1], data);
}
