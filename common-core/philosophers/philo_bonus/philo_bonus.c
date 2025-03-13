/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:02:55 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/13 17:23:29 by maxweert         ###   ########.fr       */
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

static void	eat_sleep_and_think(t_philo *philo)
{
	print_action(philo, "is eating.", 1);
	sem_wait(philo->eat_sem);
	philo->last_meal = get_current_time();
	philo->meals_eaten += 1;
	sem_post(philo->eat_sem);
	ft_usleep(philo->time_to_eat);
	sem_post(philo->forks);
	sem_post(philo->forks);
	print_action(philo, "is sleeping.", 1);
	ft_usleep(philo->time_to_sleep);
	print_action(philo, "is thinking.", 1);
}

static void	take_forks(t_philo *philo)
{
	sem_wait(philo->forks);
	print_action(philo, "has taken a fork.", 1);
	sem_wait(philo->forks);
	print_action(philo, "has taken a fork.", 1);
}

void	*philo_routine(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	while (dead_check(philo) == 0)
	{
		take_forks(philo);
		eat_sleep_and_think(philo);
	}
	printf("OK\n");
	return (NULL);
}
