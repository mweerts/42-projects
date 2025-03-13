/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 12:08:22 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/13 14:29:03 by maxweert         ###   ########.fr       */
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

typedef struct s_philo
{
	int				id;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meals_eaten;
	int				last_meal;
	int				creation_time;
	int				*dead;
	int				nb_philos;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*write_mutex;
	pthread_mutex_t	*eat_mutex;
	pthread_mutex_t	*dead_mutex;
}	t_philo;

typedef struct s_data
{
	int				nb_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_meals;
	int				dead;
	t_philo			*philo_arr;
	pthread_t		monitoring_thread;
	pthread_mutex_t	write_mutex;
	pthread_mutex_t	eat_mutex;
	pthread_mutex_t	dead_mutex;
}	t_data;

// INITIALISATION
int		init_data(int argc, char **argv, t_data *data);

// UTILS
int		ft_atoi_custom(const char *str);
int		free_data(t_data *data);
int		get_current_time(void);
void	print_action(t_philo *philo, char *str, int green);
int		ft_usleep(size_t milliseconds);

// ROUTINES

void	*philo_routine(void *ptr);
void	*monitoring_routine(void *ptr);
int		dead_check(t_philo *philo);

#endif