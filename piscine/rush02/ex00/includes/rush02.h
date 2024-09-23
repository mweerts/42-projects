/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rush02.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 15:55:34 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/22 16:37:13 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RUSH02_H
# define RUSH02_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>

typedef struct s_list
{
	struct s_list		*next;
	int					number;
	char				*str;
}	t_list;

void		ft_putstr(char *str);
int			ft_strlen(char *str);
int			ft_strcmp(char *s1, char *s2);
char		*ft_strdup(char *str);
char		*ft_strndup(char *str, int size);
char		*ft_strcpy(char *dest, char *src);
int			ft_char_isspace(char c);
int			ft_char_is_numeric(char c);
int			ft_char_is_printable(char c);
int			ft_atoi(char *str);
char		*ft_strjoin(char *s1, char *s2);
char		**ft_split(char *str, char *charset);
int			ft_power(int nb, int power);
void		ft_print(char	*input, t_list *dictionnary, t_list *multipliers);
t_list		*ft_create_elem(int nb, char *str);
void		ft_list_push_back(t_list **begin_list, int nb, char *str);
void		ft_list_push_front(t_list **begin_list, int nb, char *str);
int			ft_list_size(t_list *begin_list);
void		ft_list_free(t_list *begin_list);
void		ft_list_double_free(t_list *begin_list);
char		*ft_list_find_str(t_list *begin_list, int number);
int			ft_list_find_number(t_list *begin_list, char *str);
t_list		*ft_list_last(t_list *begin_list);
int			ft_list_key_exist(t_list *begin_list, int key);
t_list		*ft_convert_input(char *input);
int			ft_parse_dict(char *dict_path, t_list **dictionnary,
				t_list **multipliers);
char		*ft_parse_string(char *str);
int			ft_check_str_number(char *number);
int			ft_str_number_is_multiplier(char *number);
char		*ft_trim_number(char *number);
int			ft_get_multiplier(char *multiplier);
int			ft_check_dict(t_list **begin_list);
int			ft_check_multipliers(t_list **multipliers);
int			ft_free_file_content(char *file_content);
void		ft_free_result_and_input(t_list *resultat, t_list *input);
int			ft_free_dict_and_mult(t_list *dictionnary, t_list *multipliers);
#endif