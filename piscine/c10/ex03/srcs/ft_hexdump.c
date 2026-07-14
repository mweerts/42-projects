/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hexdump.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 19:52:33 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/24 15:13:49 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_hexdump.h"

void	ft_init(t_buffer *buffer)
{
	buffer->total = 0;
	buffer->bytes_read = 0;
	buffer->is_same = 0;
	buffer->only_error = 1;
	buffer->flag = 0;
}

void	ft_read(t_buffer *b, int fd, int flag)
{
	int	ret;

	while (1)
	{
		ret = read(fd, &b->buffer[b->bytes_read], 16 - b->bytes_read);
		b->bytes_read += ret;
		if (ret == 0)
			return ;
		if (b->bytes_read == 16)
		{
			if (ft_strncmp(b->previous, b->buffer, 16) == 0 && b->is_same == 0)
			{
				ft_putstr(1, "*\n");
				b->is_same = 1;
			}
			else if (ft_strncmp(b->previous, b->buffer, 16) != 0)
			{
				ft_print_memory(b->buffer, b->bytes_read, b->total, flag);
				b->is_same = 0;
			}
			b->total += b->bytes_read;
			b->bytes_read = 0;
		}
		ft_strncpy(b->previous, b->buffer, 16);
	}
}

void	ft_print_end(t_buffer *buffer, int flag)
{
	if (buffer->bytes_read > 0)
		ft_print_memory(buffer->buffer, buffer->bytes_read,
			buffer->total, flag);
	buffer->total += buffer->bytes_read;
	if (buffer->only_error == 0 && buffer->previous[0])
		ft_print_memory(0, 0, buffer->total, flag);
}

int	ft_parse_args(int argc, char **argv, t_buffer *buffer, int *i)
{
	if (argc == 1)
	{
		buffer->flag = 0;
		buffer->only_error = 0;
		ft_read(buffer, 0, buffer->flag);
	}
	else if (argc == 2 && ft_strncmp(argv[1], "-C", 3) == 0)
	{
		*i = 1;
		buffer->flag = 1;
		buffer->only_error = 0;
		ft_read(buffer, 0, buffer->flag);
	}
	else if (argc >= 2 && ft_strncmp(argv[1], "-C", 3) != 0)
		buffer->flag = 0;
	else if (argc >= 3 && ft_strncmp(argv[1], "-C", 3) == 0)
	{
		*i = 1;
		buffer->flag = 1;
	}
	else
		return (0);
	return (1);
}

int	main(int argc, char **argv)
{
	t_buffer	buffer;
	int			fd;
	int			i;

	ft_init(&buffer);
	i = 0;
	ft_parse_args(argc, argv, &buffer, &i);
	while (++i < argc)
	{
		fd = open(argv[i], O_RDONLY);
		if (fd < 0)
			ft_putstr_error(argv[i], errno, argv[0]);
		else
		{
			buffer.only_error = 0;
			ft_read(&buffer, fd, buffer.flag);
		}
		if (close(fd) == -1 && buffer.only_error && i == argc - 1)
			ft_putstr_error(argv[i], errno, argv[0]);
	}
	ft_print_end(&buffer, buffer.flag);
	return (0);
}
