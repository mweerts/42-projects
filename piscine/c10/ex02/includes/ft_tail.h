/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tail.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 19:52:52 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/20 10:33:18 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TAIL_H
# define FT_TAIL_H

# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <libgen.h>
# include <errno.h>
# include <stdlib.h>

typedef struct s_list
{
	struct s_list	*next;
	char			data;
}	t_list;

int		ft_putstr(int fd, char *str);
int		ft_putstr_error(char *path, int err, char *prog_name);
int		ft_atoi(char *str);
int		ft_check_args(int argc, char **argv);
t_list	*ft_create_elem(char data);
void	ft_list_push_back(t_list **begin_list, char data);
int		ft_list_size(t_list *begin_list);
void	ft_free_list(t_list *head);
void	ft_append_stack(t_list **head, char *append, int size);
void	ft_pop_stack(t_list **head, int maxsize);

#endif