/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 19:51:24 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/28 01:29:59 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MISC_H
# define MISC_H

//	DEBUG

void	print_data(t_data *data);
void	print_matrix(int **matrix, int height, int width);

// ERROR

void	print_err(char *msg);
void	exit_with_error(const char *msg, t_data *data);

// CLEAN

void	clean_up(t_data *data);
void	free_matrix(int **matrix, int i);

// UTILS

int		get_current_time(void);
void	count_fps(t_data *data);
int		ft_usleep(size_t ms);
void	draw_pixel(t_img *s_img, int x, int y, int color);

#endif