/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:02:55 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/14 16:30:16 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*eat_check(void *ptr)
{
	t_data *data;
	int		i;

	data = (t_data *)ptr;
	i = 0;
	while (i < data->nb_philos)
	{
		sem_wait(data->eat_sem);
		i++;
	}
	sem_wait(data->write_sem);
	printf(GREEN"\
All philosophers ate at least %d times.\n"RESET, data->nb_meals);
	sem_post(data->write_sem);
	sem_post(data->stop_sem);
	return (NULL);
}

void	*dead_check(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	while (1)
	{
		if (get_current_time() - philo->last_meal >= philo->time_to_die)
		{
			sem_wait(philo->dead_sem);
			print_action(philo, "died.", 0);
			sem_post(philo->stop_sem);
		}
	}
	return (NULL);
}

void	philo_routine(t_philo *philo)
{
	sem_wait(philo->forks);
	print_action(philo, "has taken a fork.", 1);
	sem_wait(philo->forks);
	print_action(philo, "has taken a fork.", 1);
	print_action(philo, "is eating.", 1);
	ft_usleep(philo->time_to_eat);
	philo->last_meal = get_current_time();
	if (philo->meals_eaten == philo->nb_meals)
		sem_post(philo->eat_sem);
	sem_post(philo->forks);
	sem_post(philo->forks);
	print_action(philo, "is sleeping.", 1);
	ft_usleep(philo->time_to_sleep);
	print_action(philo, "is thinking.", 1);
}
