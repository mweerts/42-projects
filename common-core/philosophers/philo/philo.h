/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 12:08:22 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/12 14:46:00 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <pthread.h>

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
}	t_philo;

typedef struct s_data
{
	int				nb_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_meals;
	t_philo			*philo_arr;
	pthread_t		monitoring_thread;
	pthread_mutex_t	write_mutex;
}	t_data;

// INITIALISATION
int		init_philos(t_data *data);
int		init_data(int argc, char **argv, t_data *data);

// UTILS
int		ft_atoi_custom(const char *str);
int		free_data(t_data *data);

// ROUTINES

void	*philo_routine(void *data);
void	*monitoring_routine(void *data);

#endif