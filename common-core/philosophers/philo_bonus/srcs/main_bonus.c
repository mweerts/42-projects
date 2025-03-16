/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 12:08:10 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/16 23:12:50 by maxweert         ###   ########.fr       */
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
	kill_processes(philos);
	free_all(philos, &data);
	return (0);
}
