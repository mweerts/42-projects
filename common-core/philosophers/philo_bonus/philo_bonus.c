/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:02:55 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/14 00:53:18 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	dead_check(t_philo *philo)
{
	sem_wait(philo->dead_sem);
	if (*philo->dead == 1)
		return (sem_post(philo->dead_sem), 1);
	sem_post(philo->dead_sem);
	return (0);
}

void	philo_routine(t_philo *philo)
{
	while (1)
	{
		sem_wait(philo->forks);
		print_action(philo, "has taken a fork.", 1);
		sem_wait(philo->forks);
		print_action(philo, "has taken a fork.", 1);
		print_action(philo, "is eating.", 1);
		usleep(philo->time_to_eat * 1000);
		philo->last_meal = get_current_time();
		philo->meals_eaten += 1;
		sem_post(philo->forks);
		sem_post(philo->forks);
		print_action(philo, "is sleeping.", 1);
		usleep(philo->time_to_sleep * 1000);
		print_action(philo, "is thinking.", 1);
	}
}
