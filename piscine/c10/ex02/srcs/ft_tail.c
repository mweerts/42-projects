/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tail.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 19:52:33 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/19 23:33:23 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_tail.h"

void	ft_putheader(char *path)
{
	ft_putstr(1, "==> ");
	ft_putstr(1, path);
	ft_putstr(1, " <==\n");
}

int	ft_tail(int fd, char *buffer, int maxsize)
{
	int		ret;
	t_list	*head;

	ret = read(fd, buffer, 256);
	head = 0;
	while (ret > 0)
	{
		ft_append_stack(&head, buffer, ret);
		ft_pop_stack(&head, maxsize);
		ret = read(fd, buffer, 256);
	}
	while (head)
	{
		write(1, &head->data, 1);
		head = head->next;
	}
	ft_free_list(head);
	if (close(fd) == -1)
		return (-1);
	return (0);
}

void	ft_put_fd(char *str, int tab_fd[2], int is_alone, char *prog_name)
{
	int	bool_format;

	bool_format = 0;
	if (tab_fd[1] > 0)
		bool_format = 1;
	if (tab_fd[0] == -1)
		ft_putstr_error(str, errno, prog_name);
	if (!is_alone && tab_fd[0] > 0)
	{
		if (tab_fd[1] > 0 || (tab_fd[1] == -1 && bool_format == 1))
			ft_putstr(2, "\n");
		ft_putheader(str);
	}
}

int	main(int argc, char **argv)
{
	char	buffer[256];
	int		i;
	int		tab_fd[2];

	i = 3;
	if (!ft_check_args(argc, argv))
		return (0);
	if (argc == 3)
		ft_tail(0, buffer, ft_atoi(argv[2]));
	tab_fd[1] = -2;
	while (i < argc)
	{
		if (argv[i])
			tab_fd[0] = open(argv[i], O_RDONLY);
		ft_put_fd(argv[i], tab_fd, argc <= 4, argv[0]);
		ft_tail(tab_fd[0], buffer, ft_atoi(argv[2]));
		if (tab_fd[0] > 0)
			tab_fd[1] = tab_fd[0];
		i++;
	}
	return (0);
}
