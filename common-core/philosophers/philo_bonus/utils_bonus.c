/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 13:42:31 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/14 00:43:16 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	ft_atoi_custom(const char *str)
{
	long	res;
	int		i;

	i = 0;
	res = 0;
	while (str[i] && ((str[i] >= 9 && str[i] <= 13) || str[i] == ' '))
		i++;
	if (str[i] == '-')
		return (-1);
	else if (str[i] == '+')
		i++;
	if (!(str[i] >= '0' && str[i] <= '9'))
		return (-1);
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + str[i] - 48;
		if (res > INT_MAX)
			return (-1);
		i++;
	}
	return ((int)res);
}

int	free_data(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		kill(data->philo_arr[i].pid, SIGKILL);
		i++;
	}
	if (data->philo_arr)
	{
		free(data->philo_arr);
		data->philo_arr = NULL;
	}
	sem_close(data->eat_sem);
	sem_close(data->write_sem);
	sem_close(data->dead_sem);
	sem_close(data->forks);
	return (1);
}

int	ft_usleep(size_t ms)
{
	size_t	start;

	start = get_current_time();
	while ((get_current_time() - start) < ms)
		usleep(500);
	return (0);
}

int	get_current_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (printf("Error: Error getting time of the day.\n"), -1);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	print_action(t_philo *philo, char *str, int green)
{
	int	time;

	sem_wait(philo->write_sem);
	time = get_current_time() - philo->creation_time;
	(void)green;
	printf(GREEN "%d" RESET " %d %s\n", time, philo->id + 1, str);
	// if (!dead_check(philo))
	// {
	// 	if (green)
	// 		printf(GREEN"%d"RESET" %d %s\n", time, philo->id + 1, str);
	// 	else
	// 		printf(RED"%d"RESET" %d %s\n", time, philo->id + 1, str);
	// }
	sem_post(philo->write_sem);
}
