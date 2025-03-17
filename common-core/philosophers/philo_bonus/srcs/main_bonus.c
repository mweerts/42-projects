/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 12:08:10 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/17 03:12:10 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	main(int argc, char **argv)
{
	t_data	data;
	t_philo	*philos;

	philos = NULL;
	if (!check_input(argc, argv))
		return (0);
	if (!init_data(&data, argc, argv))
		return (0);
	philos = init_philos(&data);
	if (!philos)
		return (0);
	if (!init_semaphores(&data))
		return (free_all(philos, &data), 0);
	if (!init_processes(philos, &data))
		return (kill_processes(philos), free_all(philos, &data), 0);
	sem_wait(data.stop_sem);
	sem_wait(data.finish_sem);
	data.finish = 1;
	sem_post(data.finish_sem);
	sem_post(data.eat_full_sem);
	ft_usleep(100);
	kill_processes(philos);
	ft_usleep(100);
	free_all(philos, &data);
	return (0);
}
