/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:25:41 by maxweert          #+#    #+#             */
/*   Updated: 2024/10/30 14:56:29 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "./libft/libft.h"
# include <stdarg.h>
# include <stdio.h>
# include <limits.h>

typedef enum e_bool
{
	FALSE,
	TRUE
}	t_bool;

typedef struct s_flags
{
	t_bool	minus;
	t_bool	plus;
	t_bool	zero;
	t_bool	hashtag;
	t_bool	space;
	int		width;
	int		precision;
}	t_flags;

int			ft_printf(const char *format, ...);
int			ft_parse_flag(va_list ap, t_flags *flags, const char *s_flag);
int			ft_print_str(va_list ap, t_flags flags);
int			ft_print_chr(va_list ap, t_flags flags);
int			ft_print_int(va_list ap, t_flags flags);
int			ft_print_uint(va_list ap, t_flags flags);
int			ft_print_hex(va_list ap, t_flags flags, char specifier);
int			ft_print_ptr(va_list ap, t_flags flags);
int			ft_print_percent(void);
int			ft_max(int nargs, ...);
void		ft_swap(char *a, char *b);

#endif