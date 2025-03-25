/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:05:14 by llebugle          #+#    #+#             */
/*   Updated: 2025/03/25 19:51:23 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "libft.h"
# include "key_linux.h"
# include "mlx.h"
# include "parser.h"
# include "structures.h"
# include "game.h"
# include "display.h"
# include "misc.h"
# include "key_linux.h"
# include <math.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

/* Constants */
# define SUCCESS 0
# define ERROR 1
# define MSG_ERR_MALLOC "No space left on device.\n"

# define WIDTH 1920
# define HEIGHT 1080
# define TEX_WIDTH 64
# define TEX_HEIGHT 64
# define MOVE_SPEED 0.05
# define ROT_SPEED 0.02
# define FOV_RATIO 0.66
# define CROSS_SIZE 10
# define CROSS_COLOR 0xFFFFFFFF

/* Colors */
# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define YELLOW "\033[0;33m"
# define BLUE "\033[34m"
# define MAGENTA "\033[35m"
# define RESET "\033[0;37m"

#endif
