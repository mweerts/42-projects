/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 12:08:10 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/15 23:34:05 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	if (!init_mutexes(philos, &data))
		return (0);
	if (!init_threads(philos, &data))
		return (free_all(philos, &data), 0);
	free_all(philos, &data);
	return (0);
}
