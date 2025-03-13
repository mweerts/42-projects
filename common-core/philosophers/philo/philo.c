/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:02:55 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/14 00:14:59 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	dead_check(t_philo *philo)
{
	pthread_mutex_lock(philo->dead_mutex);
	if (*philo->dead == 1)
		return (pthread_mutex_unlock(philo->dead_mutex), 1);
	pthread_mutex_unlock(philo->dead_mutex);
	return (0);
}

static void	eat_sleep_and_think(t_philo *philo)
{
	pthread_mutex_lock(philo->eat_mutex);
	philo->last_meal = get_current_time();
	philo->meals_eaten += 1;
	print_action(philo, "is eating.", 1);
	pthread_mutex_unlock(philo->eat_mutex);
	ft_usleep(philo->time_to_eat);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	print_action(philo, "is sleeping.", 1);
	ft_usleep(philo->time_to_sleep);
	print_action(philo, "is thinking.", 1);
}

void	*philo_routine(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	if (philo->nb_philos == 1)
	{
		pthread_mutex_lock(philo->right_fork),
		print_action(philo, "has taken a fork.", 1);
		ft_usleep(philo->time_to_die);
		pthread_mutex_unlock(philo->right_fork);
		return (NULL);
	}
	while (dead_check(philo) == 0)
	{
		if (philo->id % 2)
			ft_usleep(1);
		pthread_mutex_lock(philo->right_fork);
		print_action(philo, "has taken a fork.", 1);
		pthread_mutex_lock(philo->left_fork);
		print_action(philo, "has taken a fork.", 1);
		eat_sleep_and_think(philo);
	}
	return (NULL);
}
