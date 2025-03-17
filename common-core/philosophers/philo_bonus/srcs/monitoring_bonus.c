/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 23:10:20 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/17 03:03:29 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*check_dead(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	while (1)
	{
		sem_wait(philo->data->eat_sem);
		if (get_current_time() - philo->last_meal >= philo->data->time_to_die)
		{
			print_action(philo, DEAD);
			sem_wait(philo->data->write_sem);
			sem_post(philo->data->stop_sem);
			sem_post(philo->data->eat_sem);
		}
		sem_post(philo->data->eat_sem);
	}
	return (NULL);
}

void	*check_meals(void *ptr)
{
	t_philo	*philos;
	int		i;

	philos = (t_philo *)ptr;
	i = 0;
	while (i < philos->data->nb_philos)
	{
		sem_wait(philos->data->eat_full_sem);
		sem_wait(philos->data->finish_sem);
		if (philos->data->finish)
			return (NULL);
		sem_post(philos->data->finish_sem);
		i++;
	}
	sem_wait(philos->data->write_sem);
	printf(\
GREEN"All philosophers ate at least %d times.\n"RESET, philos->data->nb_meals);
	kill_processes(philos);
	sem_post(philos->data->stop_sem);
	return (NULL);
}
