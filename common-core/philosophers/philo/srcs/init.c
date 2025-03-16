/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 21:36:19 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/16 14:57:43 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_threads(t_philo	*philos, t_data *data)
{
	int	i;

	if (pthread_create(&data->monitoring_thread,
			NULL, monitoring_routine, philos) != 0)
		return (printf(RED"\
Error: Failed to create monitoring thread.\n"RESET), 0);
	i = -1;
	while (++i < data->nb_philos)
	{
		if (i % 2 == 0)
			ft_usleep(1);
		if (pthread_create(&philos[i].thread,
				NULL, philo_routine, &philos[i]) != 0)
			return (printf(RED"\
Error: Failed to create philo thread.\n"RESET), 0);
	}
	if (pthread_join(data->monitoring_thread, NULL) != 0)
		return (printf(RED"Error: Failed to join philo thread.\n"RESET), 0);
	i = -1;
	while (++i < data->nb_philos)
	{
		if (pthread_join(philos[i].thread, NULL) != 0)
			return (printf(\
RED"Error: Failed to join philo thread.\n"RESET), 0);
	}
	return (1);
}

int	init_mutexes(t_philo *philos, t_data *data)
{
	int	i;

	if (pthread_mutex_init(&data->write_m, NULL))
		return (printf(RED"Error: Mutex initialization failed.\n"RESET), 0);
	if (pthread_mutex_init(&data->eat_m, NULL))
		return (printf(RED"Error: Mutex initialization failed.\n"RESET), 0);
	if (pthread_mutex_init(&data->stop_m, NULL))
		return (printf(RED"Error: Mutex initialization failed.\n"RESET), 0);
	i = 0;
	while (i < data->nb_philos)
	{
		philos[i].left_fork = NULL;
		philos[i].left_fork = malloc(sizeof(pthread_mutex_t));
		if (!philos[i].left_fork)
			return (printf(RED"Error: Dynamic allocation failed.\n"RESET), 0);
		if (pthread_mutex_init(philos[i].left_fork, NULL))
			return (printf(RED"Error: Mutex initialization failed.\n"RESET), 0);
		i++;
	}
	i = -1;
	while (++i < data->nb_philos)
		philos[i].right_fork = philos[(i + 1) % data->nb_philos].left_fork;
	return (1);
}

t_philo	*init_philos(t_data *data)
{
	t_philo	*philos;
	int		i;

	philos = NULL;
	philos = malloc(sizeof(t_philo) * data->nb_philos);
	if (!philos)
		return (printf(RED"Error: Dynamic allocation failed.\n"RESET), NULL);
	i = 0;
	while (i < data->nb_philos)
	{
		philos[i].data = data;
		philos[i].id = i + 1;
		philos[i].creation_time = get_current_time();
		philos[i].meals_eaten = 0;
		philos[i].last_meal = get_current_time();
		i++;
	}
	return (philos);
}

int	init_data(t_data *data, int argc, char **argv)
{
	data->nb_meals = -1;
	data->stop = 0;
	data->nb_philos = ft_atoi(argv[1]);
	if (data->nb_philos < 1 || data->nb_philos > 200)
		return (printf(RED"Error: Wrong number of philos.\n"RESET), 0);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->nb_meals = ft_atoi(argv[5]);
	return (1);
}
