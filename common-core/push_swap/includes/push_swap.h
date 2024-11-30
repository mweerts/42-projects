/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 14:34:58 by maxweert          #+#    #+#             */
/*   Updated: 2024/11/30 17:04:28 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include "libft.h"

typedef enum e_pos
{
	TOP_A,
	TOP_B,
	BOTTOM_A,
	BOTTOM_B
}	t_pos;

typedef enum e_op
{
	pa,
	pb,
	sa,
	sb,
	ss,
	ra,
	rb,
	rr,
	rra,
	rrb,
	rrr
}	t_op;

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

typedef struct s_chunk
{
	t_pos	pos;
	int		size;
}	t_chunk;

// Sorting

void	ft_quicksort(t_data *data, t_chunk *chunk);
void	ft_sort_two(t_data *data, t_chunk *chunk);
void	ft_sort_three(t_data *data, t_chunk *chunk);

// Opti

void	ft_remove_neutrals(t_list **op_lst);

// Parsing

void	ft_parsing(t_data *data, int argc, char **argv);
int		ft_compute_rank(t_stack *stack);
t_bool	ft_check_arg(char *str);
// Utils

void	ft_init_data(t_data *data, int size);
t_bool	ft_data_issorted(t_data *data);
void	ft_move(t_data *data, t_pos from, t_pos to);
void	ft_error(t_data *data);
void	ft_free_data(t_data *data);

// Chunk

int		ft_get_chunk_elem(t_data *data, t_pos pos);
int		ft_chunk_max(t_data *data, t_chunk *chunk);
void	ft_split_chunks(t_data *data, t_chunk *chunk,
			t_chunk *min, t_chunk *max);

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