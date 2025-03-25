/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:05:41 by llebugle          #+#    #+#             */
/*   Updated: 2025/03/25 01:57:56 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "structures.h"

/* Constant */
# define ERR_RGB 2
# define DONE 1
# define VISITED 1

/* Error Messages */
# define ERR_MAP_TOO_BIG "Map too big for the screen resolution.\n"
# define MSG_WRONG_EXTENSION "Map file extension is wrong (.cub expected).\n"
# define ERR_NO_MAP "The Map couldn't be opened. Does it exist?\n"
# define ERR_EMPTY_LINES "The map has empty lines.\n"
# define ERR_NO_PLAYER_OR_EXIT "Invalid number of players on the map.\n"
# define MSG_MAP_NOT_CLOSED "The map is not closed.\n"
# define MSG_ERR_RGB "Invalid or missing colors.\
Example format : C 71,171,169.\n"
# define MSG_INVALID_CHAR "The map has an invalid element.\n"
# define MSG_MISSING_TEX "Some textures are missing or invalid.\n"
# define MSG_TOO_MANY_PLAYER "Only one player is allowed.\n"
# define MSG_NO_PLAYER "There is no player to play the game.\n"

int			process_file(t_data *data, char *filepath);
int			parse_arguments(t_data *data, int ac, char **av);
int			parse_config(t_data *data, char *line);
int			validate_config(t_data *data);
int			check_arguments(int ac, char **av);
int			parse_map(t_data *data, char **line);
int			get_map_size(t_map *map, char **line);
int			create_matrix(t_map *map, char **line);
int			is_map_closed(t_map *map);
t_texture	*load_texture(void *mlx, char *path);

#endif
