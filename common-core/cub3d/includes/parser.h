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

# define ERR_RGB 2
# define DONE 1

/* Error Messages */
# define ERR_MAP_TOO_BIG "Map too big for the screen resolution.\n"
# define ERR_WRONG_EXTENSION "Map file extension is wrong (.cub expected).\n"
# define ERR_NO_MAP "The Map couldn't be opened. Does it exist?\n"
# define ERR_EMPTY_LINES "The map has empty lines.\n"
# define ERR_NO_PLAYER_OR_EXIT "Invalid number of players on the map.\n"
# define ERR_MAP_NOT_CLOSED "The map is not closed.\n"
# define MSG_ERR_RGB "Invalid or missing colors. Example format : C 71,171,169.\n"
# define MSG_INVALID_CHAR "The map has an invalid element.\n"
# define MSG_MISSING_TEX "Some textures are missing or invalid.\n"
# define MSG_TOO_MANY_PLAYER "Only one player is allowed.\n"

int parse_arguments(t_data *data, int ac, char **av);
int	get_rgb_color(t_data *data, char *line);
int	parse_config(t_data *data, char *line);
int	validate_config(t_data *data);


// utils 
void	check_arguments(int ac, char **av, t_data *data);

#endif
