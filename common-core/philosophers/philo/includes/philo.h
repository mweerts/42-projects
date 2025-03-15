/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 12:08:22 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/16 00:02:22 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# define RESET    "\033[0m"
# define RED      "\033[31m"
# define GREEN    "\033[32m"

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <pthread.h>
# include <sys/time.h>

typedef enum e_action
{
	FORK = 0,
	EAT = 1,
	SLEEP = 2,
	THINK = 3,
	DEAD = 4
}	t_action;

typedef struct s_data
{
	int				nb_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_meals;
	int				stop;
	pthread_t		monitoring_thread;
	pthread_mutex_t	write_m;
	pthread_mutex_t	eat_m;
	pthread_mutex_t	stop_m;
}	t_data;

typedef struct s_philo
{
	int				id;
	int				creation_time;
	int				last_meal;
	int				meals_eaten;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_t		thread;
	t_data			*data;
}	t_philo;

// PARSING
int		check_input(int argc, char **argv);

// INITIALIZATION

int		init_data(t_data *data, int argc, char **argv);
t_philo	*init_philos(t_data *data);
int		init_mutexes(t_philo *philos, t_data *data);
int		init_threads(t_philo	*philos, t_data *data);

// UTILS
int		get_current_time(void);
int		ft_usleep(size_t milliseconds);
void	print_action(t_philo *philo, t_action action);
void	free_all(t_philo *philos, t_data *data);
int		ft_atoi(const char *str);

// ROUTINES

void	*philo_routine(void *ptr);
void	*monitoring_routine(void *ptr);
int		should_stop(t_data *data);
#endif