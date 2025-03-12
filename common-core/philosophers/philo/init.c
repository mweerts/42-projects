/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:20:34 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/12 19:48:03 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_threads(t_data *data)
{
	int	i;

	i = 0;
	if (pthread_create(&data->monitoring_thread, NULL, monitoring_routine,
			data) != 0)
		return (printf("Error: Failed to create monitoring thread.\n"), 0);
	while (i < data->nb_philos)
	{
		if (pthread_create(&data->philo_arr[i].thread, NULL, philo_routine,
				&data->philo_arr[i]) != 0)
			return (free_data(data), printf("\
Error: Failed to create philosopher thread.\n"), 0);
		i++;
	}
	if (pthread_join(data->monitoring_thread, NULL) != 0)
		return (printf("Error: Error joining thread.\n"), free_data(data), 0);
	i = 0;
	while (i < data->nb_philos)
	{
		if (pthread_join(data->philo_arr[i].thread, NULL) != 0)
			return (printf("Error: Error joining thread.\n"), free_data(data),
				0);
		i++;
	}
	return (1);
}

static int	init_mutex(t_data *data)
{
	int	i;

	i = 0;
	pthread_mutex_init(&data->write_mutex, NULL);
	pthread_mutex_init(&data->eat_mutex, NULL);
	pthread_mutex_init(&data->dead_mutex, NULL);
	while (i < data->nb_philos)
	{
		data->philo_arr[i].left_fork = malloc(sizeof(pthread_mutex_t));
		if (!data->philo_arr[i].left_fork)
			return (free_data(data), 0);
		pthread_mutex_init(data->philo_arr[i].left_fork, NULL);
		data->philo_arr[i].write_mutex = &data->write_mutex;
		data->philo_arr[i].eat_mutex = &data->eat_mutex;
		data->philo_arr[i].dead_mutex = &data->dead_mutex;
		i++;
	}
	i = 0;
	while (i < data->nb_philos)
	{
		data->philo_arr[i].right_fork = data->philo_arr[(i + 1)
			% data->nb_philos].left_fork;
		i++;
	}
	return (init_threads(data));
}

static int	init_philos(t_data *data)
{
	int	i;

	i = 0;
	data->philo_arr = malloc(sizeof(t_philo) * data->nb_philos);
	if (!data->philo_arr)
		return (printf("Error: Memory allocation failed.\n"), 0);
	while (i < data->nb_philos)
	{
		data->philo_arr[i].id = i;
		data->philo_arr[i].meals_eaten = 0;
		data->philo_arr[i].left_fork = NULL;
		data->philo_arr[i].right_fork = NULL;
		data->philo_arr[i].creation_time = get_current_time();
		data->philo_arr[i].last_meal = get_current_time();
		data->philo_arr[i].time_to_eat = data->time_to_eat;
		data->philo_arr[i].time_to_sleep = data->time_to_sleep;
		data->philo_arr[i].time_to_die = data->time_to_die;
		data->philo_arr[i].dead = &data->dead;
		data->philo_arr[i].nb_philos = data->nb_philos;
		i++;
	}
	return (init_mutex(data));
}

int	init_data(int argc, char **argv, t_data *data)
{
	int	i;
	int	tmp;

	i = 0;
	data->nb_meals = -1;
	data->dead = 0;
	while (++i < argc)
	{
		tmp = ft_atoi_custom(argv[i]);
		if (tmp == -1)
			return (printf("Error: Invalid argument: \"%s\".\n", argv[i]), 0);
		if (i == 1 && tmp == 0)
			return (printf("\
Error: Wrong number of Philosophers: \"%s\".\n", argv[i]), 0);
		if (i == 1)
			data->nb_philos = tmp;
		if (i == 2)
			data->time_to_die = tmp;
		if (i == 3)
			data->time_to_eat = tmp;
		if (i == 4)
			data->time_to_sleep = tmp;
		if (i == 5)
			data->nb_meals = tmp;
	}
	return (init_philos(data));
}
