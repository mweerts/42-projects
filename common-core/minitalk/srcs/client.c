/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 18:49:29 by maxweert          #+#    #+#             */
/*   Updated: 2024/11/19 20:18:40 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

t_bool	g_received = FALSE;

static void	send_string(char *str, pid_t pid)
{
	int	i;
	int	j;

	i = 0;
	while (str[i])
	{
		j = 8;
		while (--j >= 0)
		{
			if ((str[i] >> j) & 1)
				kill(pid, SIGUSR2);
			else
				kill(pid, SIGUSR1);
			while (1)
			{
				if (g_received)
				{
					g_received = FALSE;
					usleep(1);
					break ;
				}
			}
		}
		i++;
	}
}

static void	sigusr_handler(int signal)
{
	g_received = TRUE;
	if (signal == SIGUSR1)
		write(1, "Received 1\n", 11);
	if (signal == SIGUSR2)
		write(1, "Received 0\n", 11);
}

int	main(int argc, char **argv)
{
	pid_t				pid;

	if (argc != 3)
	{
		ft_printf("Usage: ./%s <PID> <Text>\n", argv[0]);
		return (0);
	}
	pid = ft_atoi(argv[1]);
	if (pid <= 0)
	{
		ft_printf("PID Error !\n");
		return (0);
	}
	signal(SIGUSR1, sigusr_handler);
	signal(SIGUSR2, sigusr_handler);
	send_string(argv[2], pid);
}
