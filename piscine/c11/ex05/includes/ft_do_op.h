/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_do_op.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 13:38:16 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/15 14:39:37 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_DO_OP_H
# define FT_DO_OP_H
# include <unistd.h>

typedef void	t_function (int a, int b);

int		ft_strlen(char *str);
void	ft_putstr(char *str);
void	ft_putnbr(int nb);
int		ft_atoi(char *str);
void	ft_addition(int a, int b);
void	ft_soustraction(int a, int b);
void	ft_multiplication(int a, int b);
void	ft_division(int a, int b);
void	ft_modulo(int a, int b);

#endif