/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memory.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 10:35:05 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/23 11:50:34 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_hexdump.h"

void	*ft_realloc(int old_size, int append_size, void *old_ptr)
{
	char	*ret;
	int		i;
	char	*tmp;

	i = 0;
	ret = malloc(old_size + append_size + 1);
	if (!ret)
		return (0);
	ft_memset(ret, old_size + append_size + 1);
	tmp = (char *)old_ptr;
	while (tmp[i])
	{
		ret[i] = tmp[i];
		i++;
	}
	ret[i] = '\0';
	free(old_ptr);
	return (ret);
}

void	*ft_calloc(int size)
{
	char	*ret;

	ret = malloc(size);
	if (!ret)
		return (0);
	ft_memset(ret, size);
	return (ret);
}

void	ft_memset(char *ptr, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		ptr[i] = 0;
		i++;
	}
}

void	ft_swap(char *ptr1, char *ptr2)
{
	char	*tmp;

	tmp = ptr1;
	ptr1 = ptr2;
	ptr2 = tmp;
}

int	ft_bufflen(char *buffer)
{
	int	i;

	i = 0;
	while (i < 16 && buffer[i])
		i++;
	return (i);
}
