/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_map.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 22:08:24 by apierret          #+#    #+#             */
/*   Updated: 2024/09/25 17:18:58 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_bsq.h"

int	ft_init_map(char *read_buffer, t_map **map, int *i, int *first_read)
{
	int	j;

	*first_read = 0;
	j = 0;
	while (read_buffer[*i] && read_buffer[*i] != '\n')
		(*i)++;
	while (j < *i -3 && ft_char_is_numeric(read_buffer[j]))
		j++;
	if (!ft_malloc_map(map, ft_light_atoi(read_buffer, j)))
		return (0);
	if (read_buffer[*i -3] != '\n' && read_buffer[*i -2] != '\n'
		&& read_buffer[*i -1] != '\n' && read_buffer[*i] == '\n')
	{
		(*map)->empty = read_buffer[*i -3];
		(*map)->obstacle = read_buffer[*i -2];
		(*map)->filled = read_buffer[*i -1];
		(*i)++;
		return (ft_different_char(*map));
	}
	return (0);
}

int	ft_copy_line(t_map **map, t_read_data *data)
{
	if (!ft_malloc_map_grid(map, data->line_len))
		return (0);
	(*map)->cols = data->line_len;
	ft_strcpy((*map)->map[data->row], data->line);
	data->row++;
	data->col = 0;
	data->i++;
	free(data->line);
	return (1);
}

int	ft_read_first_line(char *r_buf, t_map **map, t_read_data *data)
{
	char	c;

	while (data->i < data->bytes)
	{
		c = r_buf[data->i];
		if (c == '\n')
			return (ft_copy_line(map, data));
		else
		{
			data->line = ft_realloc(data->line, data->line_len + 2);
			if (!data->line)
				return (0);
			data->line[data->line_len] = c;
			data->line_len++;
			data->line[data->line_len] = '\0';
		}
		data->i++;
	}
	return (1);
}

int	ft_fill_map(char *r_buf, t_map **map, t_read_data *data)
{
	char	c;

	if ((*map)->cols == 0 && !ft_read_first_line(r_buf, map, data))
		return (0);
	while (data->i < data->bytes && data->row < (*map)->rows)
	{
		c = r_buf[data->i];
		if (c == '\n')
		{
			if (data->col != (*map)->cols)
				return (0);
			data->row++;
			data->col = 0;
		}
		else
		{
			if (data->col < (*map)->cols && ft_validate_char(c, *map))
				(*map)->map[data->row][data->col++] = c;
			else
				return (0);
		}
		data->i++;
	}
	return (data->i >= data->bytes);
}
