/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hexdump.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 19:52:52 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/24 11:42:50 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_HEXDUMP_H
# define FT_HEXDUMP_H

# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <libgen.h>
# include <errno.h>
# include <stdlib.h>
# include <stdio.h>

typedef struct s_buffer
{
	char	buffer[16];
	char	previous[16];
	int		total;
	int		bytes_read;
	int		is_same;
	int		only_error;
	int		flag;
}	t_buffer;

void	ft_putchar(char c);
int		ft_putstr(int fd, char *str);
int		ft_strlen(char *str);
int		ft_putstr_error(char *path, int err, char *prog_name);
char	*ft_strncat(char *dest, char *src, int size);
void	ft_print_hex_number(unsigned long nbr, int str_size);
void	*ft_calloc(int size);
void	*ft_realloc(int old_size, int append_size, void *old_ptr);
void	ft_memset(char *ptr, int size);
void	ft_swap(char *ptr1, char *ptr2);
int		ft_check_args(int argc, char **argv);
int		ft_bufflen(char *buffer);
char	*ft_strncpy(char *dest, char *src, unsigned int n);
int		ft_strncmp(char *s1, char *s2, int size);
char	*ft_strdup(char *str);
void	ft_print_memory_noflag(void *addr, unsigned int size, int line);
void	ft_print_memory_flag(void *addr, unsigned int size, int line);
void	ft_print_memory(void *addr, unsigned int size, int line, int flag);
void	ft_print_content_hex(unsigned char *addr, unsigned int size);
void	ft_print_content_hex_noflag(unsigned char *addr, unsigned int size);

#endif