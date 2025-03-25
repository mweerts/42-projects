/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 16:21:13 by llebugle          #+#    #+#             */
/*   Updated: 2025/03/25 18:45:09 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	print_err(char *msg)
{
	ft_printf_fd(STDERR_FILENO, YELLOW);
	ft_printf_fd(STDERR_FILENO, "Error\n");
	if (errno == ENOMEM)
		perror(strerror(errno));
	else if (msg)
		ft_printf_fd(2, "%s", msg, RESET);
	ft_printf_fd(2, RESET);
}

void	exit_with_error(const char *msg, t_data *data)
{
	ft_printf_fd(2, YELLOW);
	ft_printf_fd(2, "Error\n");
	if (errno == ENOMEM)
		perror(strerror(errno));
	else if (msg)
		ft_printf_fd(2, "%s", msg, RESET);
	ft_printf_fd(2, RESET);
	clean_up(data);
	exit(EXIT_FAILURE);
}
