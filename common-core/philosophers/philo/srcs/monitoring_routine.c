/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring_routine.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 23:10:20 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/15 23:55:24 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	should_stop(t_data *data)
{
	int	ret;

	ret = 0;
	pthread_mutex_lock(&data->stop_m);
	if (data->stop == 1)
		ret = 1;
	pthread_mutex_unlock(&data->stop_m);
	return (ret);
}

static int	check_dead(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < philos->data->nb_philos)
	{
		pthread_mutex_lock(&philos->data->eat_m);
		if (get_current_time() - philos[i].last_meal
			>= philos->data->time_to_die)
		{
			pthread_mutex_unlock(&philos->data->eat_m);
			print_action(&philos[i], DEAD);
			pthread_mutex_lock(&philos->data->stop_m);
			philos->data->stop = 1;
			pthread_mutex_unlock(&philos->data->stop_m);
			return (1);
		}
		pthread_mutex_unlock(&philos->data->eat_m);
		i++;
	}
	return (0);
}

static int	check_all_have_eat(t_philo *philos)
{
	int	i;

	i = 0;
	if (philos->data->nb_meals == -1)
		return (0);
	while (i < philos->data->nb_philos)
	{
		pthread_mutex_lock(&philos->data->eat_m);
		if (!(philos[i].meals_eaten >= philos->data->nb_meals))
			return (pthread_mutex_unlock(&philos->data->eat_m), 0);
		pthread_mutex_unlock(&philos->data->eat_m);
		i++;
	}
	pthread_mutex_lock(&philos->data->stop_m);
	philos->data->stop = 1;
	pthread_mutex_unlock(&philos->data->stop_m);
	return (1);
}

void	*monitoring_routine(void *ptr)
{
	t_philo	*philos;

	philos = (t_philo *)ptr;
	while (1)
	{
		if (check_dead(philos))
			break ;
		if (check_all_have_eat(philos))
		{
			pthread_mutex_lock(&philos->data->write_m);
			printf(\
GREEN"All philosophers ate at least %d times.\n"RESET, philos->data->nb_meals);
			pthread_mutex_unlock(&philos->data->write_m);
			break ;
		}
	}
	return (NULL);
}
