/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:20:34 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/14 00:53:02 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static int	init_processes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		data->philo_arr[i].pid = fork();
		if (data->philo_arr[i].pid == 0)
		{
			philo_routine(data->philo_arr + i);
			exit(0);
		}
		i++;
	}
	waitpid(-1, NULL, 0);
	return (1);
}

static int	init_semaphores(t_data *data)
{
	int	i;

	i = 0;
	sem_unlink("eat_sem");
	sem_unlink("write_sem");
	sem_unlink("dead_sem");
	sem_unlink("forks");
	data->eat_sem = sem_open("eat_sem", O_CREAT, 0644, 1);
	data->write_sem = sem_open("write_sem", O_CREAT, 0644, 1);
	data->dead_sem = sem_open("dead_sem", O_CREAT, 0644, 1);
	data->forks = sem_open("forks", O_CREAT, 0644, data->nb_philos);
	while (i < data->nb_philos)
	{
		data->philo_arr[i].eat_sem = data->eat_sem;
		data->philo_arr[i].write_sem = data->write_sem;
		data->philo_arr[i].dead_sem = data->dead_sem;
		data->philo_arr[i].forks = data->forks;
		i++;
	}
	return (init_processes(data));
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
		data->philo_arr[i].creation_time = get_current_time();
		data->philo_arr[i].last_meal = get_current_time();
		data->philo_arr[i].time_to_eat = data->time_to_eat;
		data->philo_arr[i].time_to_sleep = data->time_to_sleep;
		data->philo_arr[i].time_to_die = data->time_to_die;
		data->philo_arr[i].dead = &data->dead;
		data->philo_arr[i].nb_philos = data->nb_philos;
		data->philo_arr[i].pid = -1;
		i++;
	}
	return (init_semaphores(data));
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
