/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 13:42:31 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/15 23:41:58 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

void	free_all(t_philo *philos, t_data *data)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&data->write_m);
	pthread_mutex_destroy(&data->eat_m);
	while (philos && i < data->nb_philos)
	{
		pthread_mutex_destroy(philos[i].left_fork);
		free(philos[i].left_fork);
		i++;
	}
	if (philos)
		free(philos);
}

void	print_action(t_philo *philo, t_action action)
{
	int	time;

	time = get_current_time() - philo->creation_time;
	pthread_mutex_lock(&philo->data->write_m);
	if (should_stop(philo->data))
	{
		pthread_mutex_unlock(&philo->data->write_m);
		return ;
	}
	if (action == DEAD)
		printf(RED"[%d] %d died.\n"RESET, time, philo->id);
	else if (action == FORK)
		printf(GREEN"[%d] %d has taken a fork.\n"RESET, time, philo->id);
	else if (action == EAT)
		printf(GREEN"[%d] %d is eating.\n"RESET, time, philo->id);
	else if (action == SLEEP)
		printf(GREEN"[%d] %d is sleeping.\n"RESET, time, philo->id);
	else if (action == THINK)
		printf(GREEN"[%d] %d is thinking.\n"RESET, time, philo->id);
	pthread_mutex_unlock(&philo->data->write_m);
}

int	ft_atoi(const char *str)
{
	int		ret;
	int		sign;
	size_t	i;

	ret = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || str[i] == '\f' || str[i] == '\n'
		|| str[i] == '\r' || str[i] == '\t' || str[i] == '\v')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		ret = (ret * 10) + str[i] - '0';
		i++;
	}
	ret *= sign;
	return (ret);
}
