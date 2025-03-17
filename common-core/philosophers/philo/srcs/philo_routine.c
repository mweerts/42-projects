/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 21:02:02 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/17 19:51:35 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	philo_take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print_action(philo, FORK);
		pthread_mutex_lock(philo->left_fork);
		print_action(philo, FORK);
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_action(philo, FORK);
		pthread_mutex_lock(philo->right_fork);
		print_action(philo, FORK);
	}
}

static void	philo_eat(t_philo *philo)
{
	print_action(philo, EAT);
	pthread_mutex_lock(&philo->data->eat_m);
	philo->last_meal = get_current_time();
	philo->meals_eaten += 1;
	pthread_mutex_unlock(&philo->data->eat_m);
	ft_usleep(philo->data->time_to_eat);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

static void	philo_sleep(t_philo *philo)
{
	print_action(philo, SLEEP);
	ft_usleep(philo->data->time_to_sleep);
}

static void	philo_think(t_philo *philo, int print)
{
	int	time_to_think;

	pthread_mutex_lock(&philo->data->eat_m);
	time_to_think = (philo->data->time_to_die
			- (get_current_time() - philo->last_meal)
			- philo->data->time_to_eat) / 2;
	pthread_mutex_unlock(&philo->data->eat_m);
	if (time_to_think < 0)
		time_to_think = 0;
	if (time_to_think == 0 && print == 1)
		time_to_think = 1;
	if (time_to_think > 600)
		time_to_think = 200;
	if (print == 1)
		print_action(philo, THINK);
	ft_usleep(time_to_think);
}

void	*philo_routine(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	if (philo->data->nb_philos == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		print_action(philo, FORK);
		ft_usleep(philo->data->time_to_die);
		pthread_mutex_unlock(philo->left_fork);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		philo_think(philo, 0);
	while (!should_stop(philo->data))
	{
		philo_take_forks(philo);
		philo_eat(philo);
		philo_sleep(philo);
		philo_think(philo, 1);
	}
	return (NULL);
}
