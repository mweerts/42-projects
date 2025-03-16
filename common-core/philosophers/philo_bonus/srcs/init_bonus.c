/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 21:36:19 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/17 00:13:19 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	init_processes(t_philo	*philos, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		philos[i].pid = fork();
		if (philos[i].pid == -1)
			return (printf(RED"Error: Fork failed.\n"RESET), 0);
		if (philos[i].pid == 0)
		{
			if (pthread_create(&philos->dead_monitor, NULL, check_dead, &philos[i]) != 0)
				return (printf(RED"Error: Failed to create dead monitoring thread.\n"RESET), 0);
			if (pthread_detach(philos->dead_monitor) != 0)
				return (printf(RED"Error: Failed to detach dead monitoring thread.\n"RESET), 0);
			philo_routine(&philos[i]);
			exit(0);
		}
		i++;
	}
	if (data->nb_meals != -1)
	{
		if (pthread_create(&data->meals_monitor, NULL, check_meals, philos) != 0)
			return (printf(RED"Error: Failed to create meals monitoring thread.\n"RESET), 0);
		if (pthread_detach(data->meals_monitor) != 0)
			return (printf(RED"Error: Failed to detach meals monitoring thread.\n"RESET), 0);
	}
	return (1);
}

int	init_semaphores(t_data *data)
{
	sem_unlink("write_sem");
	sem_unlink("eat_sem");
	sem_unlink("stop_sem");
	sem_unlink("forks_sem");
	sem_unlink("eat_full_sem");
	data->write_sem = sem_open("write_sem", O_CREAT, 0644, 1);
	data->eat_sem = sem_open("eat_sem", O_CREAT, 0644, 1);
	data->stop_sem = sem_open("stop_sem", O_CREAT, 0644, 0);
	data->forks_sem = sem_open("forks_sem", O_CREAT, 0644, data->nb_philos);
	data->eat_full_sem = sem_open("eat_full_sem", O_CREAT, 0644, 0);
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
		philos[i].eat_ok = 0;
		philos[i].last_meal = get_current_time();
		i++;
	}
	return (philos);
}

int	init_data(t_data *data, int argc, char **argv)
{
	data->nb_meals = -1;
	data->nb_philos = ft_atoi(argv[1]);
	data->write_sem = NULL;
	data->eat_sem = NULL;
	data->stop_sem = NULL;
	data->forks_sem = NULL;
	if (data->nb_philos < 1 || data->nb_philos > 200)
		return (printf(RED"Error: Wrong number of philos.\n"RESET), 0);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->nb_meals = ft_atoi(argv[5]);
	return (1);
}
