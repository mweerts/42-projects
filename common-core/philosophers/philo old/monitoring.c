/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 19:54:11 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/13 14:29:08 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_dead(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		pthread_mutex_lock(data->philo_arr[i].eat_mutex);
		if (get_current_time() - data->philo_arr[i].last_meal
			>= data->time_to_die)
		{
			pthread_mutex_unlock(data->philo_arr[i].eat_mutex);
			print_action(&data->philo_arr[i], "died.", 0);
			pthread_mutex_lock(&data->dead_mutex);
			data->dead = 1;
			pthread_mutex_unlock(&data->dead_mutex);
			return (1);
		}
		pthread_mutex_unlock(data->philo_arr[i].eat_mutex);
		i++;
	}
	return (0);
}

static int	check_all_have_eat(t_data *data)
{
	int	i;

	i = 0;
	if (data->nb_meals == -1)
		return (0);
	while (i < data->nb_philos)
	{
		pthread_mutex_lock(data->philo_arr[i].eat_mutex);
		if (!(data->philo_arr[i].meals_eaten >= data->nb_meals))
			return (pthread_mutex_unlock(data->philo_arr[i].eat_mutex), 0);
		pthread_mutex_unlock(data->philo_arr[i].eat_mutex);
		i++;
	}
	pthread_mutex_lock(&data->dead_mutex);
	data->dead = 1;
	pthread_mutex_unlock(&data->dead_mutex);
	return (1);
}

void	*monitoring_routine(void *ptr)
{
	t_data	*data;

	data = (t_data *)ptr;
	while (1)
	{
		if (check_dead(data))
			break ;
		if (check_all_have_eat(data))
		{
			pthread_mutex_lock(&data->write_mutex);
			printf(GREEN"\
All philosophers ate at least %d times.\n"RESET, data->nb_meals);
			pthread_mutex_unlock(&data->write_mutex);
			break ;
		}
	}
	return (NULL);
}
