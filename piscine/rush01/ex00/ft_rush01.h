/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rush01.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 12:24:25 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/14 20:01:46 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_RUSH01_H
# define FT_RUSH01_H

# include <unistd.h>
# include <stdlib.h>

void	ft_putchar(char c);
void	ft_putstr(char *str);
int		ft_strlen(char *str);
void	ft_init_board(int board[4][4]);
void	ft_print_board(int board[4][4]);
int		parse_arguments(char *str, int tab[4][4]);
void	ft_set_tab_zero(int tab[4]);
int		ft_check_row_left(int board[4][4], int row, int left_view);
int		ft_check_row_right(int board[4][4], int row, int right_view);
int		ft_check_col_up(int board[4][4], int col, int up_view);
int		ft_check_col_down(int board[4][4], int col, int down_view);
int		ft_is_valid(int board[4][4], int view[4][4]);
int		ft_solve(int board[4][4], int view[4][4], int row, int col);
#endif