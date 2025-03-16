/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 21:02:02 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/16 23:12:25 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	philo_take_forks(t_philo *philo)
{
	sem_wait(philo->data->forks_sem);
	print_action(philo, FORK);
	sem_wait(philo->data->forks_sem);
	print_action(philo, FORK);
}

static void	philo_eat(t_philo *philo)
{
	print_action(philo, EAT);
	sem_wait(philo->data->eat_sem);
	philo->last_meal = get_current_time();
	philo->meals_eaten += 1;
	if (philo->data->nb_meals != -1 && philo->meals_eaten
		>= philo->data->nb_meals && !philo->eat_ok)
	{
		philo->eat_ok = 1;
		sem_post(philo->data->eat_full_sem);
	}
	sem_post(philo->data->eat_sem);
	ft_usleep(philo->data->time_to_eat);
	sem_post(philo->data->forks_sem);
	sem_post(philo->data->forks_sem);
}

static void	philo_sleep(t_philo *philo)
{
	print_action(philo, SLEEP);
	ft_usleep(philo->data->time_to_sleep);
}

static void	philo_think(t_philo *philo)
{
	print_action(philo, THINK);
}

void	*philo_routine(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	if (philo->data->nb_philos == 1)
	{
		sem_wait(philo->data->forks_sem);
		print_action(philo, FORK);
		ft_usleep(philo->data->time_to_die);
		sem_post(philo->data->forks_sem);
		return (NULL);
	}
	while (1)
	{
		philo_take_forks(philo);
		philo_eat(philo);
		philo_sleep(philo);
		philo_think(philo);
	}
	return (NULL);
}
