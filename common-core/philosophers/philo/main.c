/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 12:08:10 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/12 14:48:51 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_data	data;

	if (!init_data(argc, argv, &data))
		return (0);
	printf("%d %d %d %d %d\n", data.nb_philos, data.time_to_die, data.time_to_eat, data.time_to_sleep, data.nb_meals);

	for (int i = 0; i < data.nb_philos; i++)
		printf("Philo %d : %p %p\n", i, data.philo_arr[i].left_fork, data.philo_arr[i].right_fork); 
}
