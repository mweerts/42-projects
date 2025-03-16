/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 22:05:02 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/16 23:09:58 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	free_all(t_philo *philos, t_data *data)
{
	if (data->write_sem && data->write_sem != SEM_FAILED)
		sem_close(data->write_sem);
	if (data->eat_sem && data->eat_sem != SEM_FAILED)
		sem_close(data->eat_sem);
	if (data->stop_sem && data->stop_sem != SEM_FAILED)
		sem_close(data->stop_sem);
	if (data->forks_sem && data->forks_sem != SEM_FAILED)
		sem_close(data->forks_sem);
	if (data->eat_full_sem && data->eat_full_sem != SEM_FAILED)
		sem_close(data->eat_full_sem);
	if (philos)
		free(philos);
}

void	kill_processes(t_philo *philos)
{
	int	i;
	int	status;

	i = 0;
	while (i < philos->data->nb_philos)
	{
		kill(philos[i].pid, SIGKILL);
		i++;
	}
}
