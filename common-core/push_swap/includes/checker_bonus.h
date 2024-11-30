/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_bonus.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 14:34:58 by maxweert          #+#    #+#             */
/*   Updated: 2024/11/30 17:18:40 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHECKER_BONUS_H
# define CHECKER_BONUS_H

# include "libft.h"

typedef struct s_stack
{
	int	*stack;
	int	size;
}	t_stack;

typedef struct s_data
{
	t_stack	stack_a;
	t_stack	stack_b;
	t_list	*op_lst;
}	t_data;

// Parsing

void	ft_parsing(t_data *data, int argc, char **argv);
int		ft_compute_rank(t_stack *stack);
t_bool	ft_check_arg(char *str);

// Utils

void	ft_init_data(t_data *data, int size);
t_bool	ft_data_issorted(t_data *data);
void	ft_error(t_data *data);
void	ft_free_data(t_data *data);

// Stack Operations

int		ft_map_op(t_data *data, char *op, t_bool print);
void	ft_pa(t_data *data);
void	ft_pb(t_data *data);
void	ft_sa(t_data *data);
void	ft_sb(t_data *data);
void	ft_ss(t_data *data);
void	ft_ra(t_data *data);
void	ft_rb(t_data *data);
void	ft_rr(t_data *data);
void	ft_rra(t_data *data);
void	ft_rrb(t_data *data);
void	ft_rrr(t_data *data);

#endif