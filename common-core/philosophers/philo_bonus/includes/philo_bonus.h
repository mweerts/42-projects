/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 12:08:22 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/17 02:56:13 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# define RESET		"\033[0m"
# define RED		"\033[31m"
# define GREEN		"\033[32m"
# define YELLOW		"\033[33m"
# define BLUE		"\033[34m"
# define CYAN		"\033[36m"
# define PURPLE		"\033[35m"

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <pthread.h>
# include <sys/time.h>
# include <semaphore.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>

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
	int				finish;
	pthread_t		meals_monitor;
	sem_t			*write_sem;
	sem_t			*eat_sem;
	sem_t			*eat_full_sem;
	sem_t			*stop_sem;
	sem_t			*forks_sem;
	sem_t			*finish_sem;
}	t_data;

typedef struct s_philo
{
	t_data			*data;
	int				id;
	int				creation_time;
	int				last_meal;
	int				meals_eaten;
	int				eat_ok;
	int				pid;
	pthread_t		dead_monitor;
}	t_philo;

// PARSING
int		check_input(int argc, char **argv);

// INITIALIZATION

int		init_data(t_data *data, int argc, char **argv);
t_philo	*init_philos(t_data *data);
int		init_semaphores(t_data *data);
int		init_processes(t_philo	*philos, t_data *data);

// UTILS
int		get_current_time(void);
int		ft_usleep(size_t milliseconds);
void	print_action(t_philo *philo, t_action action);
int		ft_atoi(const char *str);

// EXIT

void	free_all(t_philo *philos, t_data *data);
void	kill_processes(t_philo *philos);

// ROUTINES

void	*philo_routine(void *ptr);
void	*check_dead(void *ptr);
void	*check_meals(void *ptr);
#endif