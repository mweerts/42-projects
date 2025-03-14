/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:05:41 by llebugle          #+#    #+#             */
/*   Updated: 2025/03/13 19:05:42 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

#include "structures.h"

/* Error Messages */
# define ERR_MAP_TOO_BIG "Map too big for the screen resolution.\n"
# define ERR_WRONG_EXTENSION "Map file extension is wrong (.cub expected).\n"
# define ERR_NO_MAP "The Map couldn't be opened. Does it exist?\n"
# define ERR_EMPTY_LINES "The map has empty lines.\n"
# define ERR_INVALID_CHAR "The map has an invalid element.\n"
# define ERR_NO_PLAYER_OR_EXIT "Invalid number of players on the map.\n"
# define ERR_MAP_NOT_CLOSED "The map is not closed.\n"
# define ERR_RGB_FORMAT "Wrong color format. Expected rgb (i.e 255,16,64).\n"

int parse_arguments(t_data *data, int ac, char **av);
int	get_rgb_color(t_data *data, char *line, char elem);

#endif
