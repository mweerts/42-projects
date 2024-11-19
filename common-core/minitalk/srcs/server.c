/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 18:49:29 by maxweert          #+#    #+#             */
/*   Updated: 2024/11/19 20:30:03 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

t_bool	g_received = FALSE;

static void	sigusr_handler(int signal, siginfo_t *info, void *context)
{
	static unsigned char	c;
	static int				i;

	g_received = TRUE;
	if (signal == SIGUSR2)
		c = c | (1 << (7 - i));
	i++;
	if (i == 8)
	{
		write(1, &c, 1);
		i = 0;
		c = 0;
	}
	if (signal == SIGUSR1)
		kill(info->si_pid, SIGUSR1);
	else if (signal == SIGUSR2)
		kill(info->si_pid, SIGUSR2);
	if (context)
		context = NULL;
}

int	main(void)
{
	pid_t				pid;
	struct sigaction	sa;

	pid = getpid();
	ft_printf("%d\n", pid);
	sa.sa_sigaction = &sigusr_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	while (1)
	{
		if (g_received)
		{
			g_received = FALSE;
			usleep(1);
		}
	}
	return (0);
}
