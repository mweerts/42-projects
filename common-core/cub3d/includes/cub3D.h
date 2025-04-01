/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:05:14 by llebugle          #+#    #+#             */
/*   Updated: 2025/03/28 20:34:51 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "libft.h"
# include "assets.h"
# include "key_linux.h"
# include "mlx.h"
# include "parser.h"
# include "structures.h"
# include "game.h"
# include "display.h"
# include "misc.h"
# include <math.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <semaphore.h>
# include <X11/keysym.h>
# include <X11/keysymdef.h>

# ifndef BONUS
#  define BONUS 1
# endif

/* Constants */
# define SUCCESS 0
# define ERROR 1
# define MSG_ERR_MALLOC "No space left on device.\n"

# define WIDTH 1600
# define HEIGHT 900
# define MOVE_SPEED 0.07
# define ROT_SPEED 0.05
# define MOUSE_SENSIBILITY 0.0005
# define FOV_RATIO 0.66
# define POINTER_SIZE 12
# define POINTER_COLOR 0xFFFFFFFF
# define MAIN_BLUE 0x00DBDB
# define TEXT_COLOR 0xFFFFFFFF
# define ANIM_SPEED 5
# define TIME_LEFT_IN_MS 600000 //10 * 60 * 1000 
# define WIN 1
# define LOOSE 2
# define FRAME_MARGIN 50
# define MAP_CELL_SIZE 14

/* Colors */
# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define YELLOW "\033[0;33m"
# define BLUE "\033[34m"
# define MAGENTA "\033[35m"
# define RESET "\033[0;37m"

// MAIN
int		leave(t_data *data);

#endif
