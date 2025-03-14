/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:05:14 by llebugle          #+#    #+#             */
/*   Updated: 2025/03/13 19:05:17 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../libft/libft.h"
# include "../minilibx-linux/mlx.h"
# include "parser.h"
# include "structures.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

/* Constants */
# define SUCCESS 0
# define ERROR 1

/* Colors */
# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define YELLOW "\033[0;33m"
# define RESET "\033[0;37m"

/* Error Messages */
# define ERR_MAP_TOO_BIG "Map too big for the screen resolution.\n"
# define ERR_WRONG_EXTENSION "Map file extension is wrong (.cub expected).\n"
# define ERR_NO_MAP "The Map couldn't be opened. Does it exist?\n"
# define ERR_EMPTY_LINES "The map has empty lines.\n"
# define ERR_INVALID_CHAR "The map has an invalid element.\n"
# define ERR_NO_PLAYER_OR_EXIT "Invalid number of players on the map.\n"
# define ERR_MAP_NOT_CLOSED "The map is not closed.\n"

void	display_err_and_exit(const char *msg, t_data *data);
void	clean_up(t_data *data);

#endif
