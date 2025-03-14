/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 12:08:22 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/14 16:22:04 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# define RESET    "\033[0m"
# define RED      "\033[31m"
# define GREEN    "\033[32m"

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

typedef struct s_philo
{
	int				id;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meals_eaten;
	int				nb_meals;
	int				last_meal;
	int				creation_time;
	int				nb_philos;
	int				pid;
	sem_t			*forks;
	sem_t			*eat_sem;
	sem_t			*write_sem;
	sem_t			*dead_sem;
	sem_t			*stop_sem;
}	t_philo;

typedef struct s_data
{
	int				nb_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_meals;
	t_philo			*philo_arr;
	sem_t			*forks;
	sem_t			*eat_sem;
	sem_t			*write_sem;
	sem_t			*dead_sem;
	sem_t			*stop_sem;
	pthread_t		dead_thread;
	pthread_t		eat_check_thread;
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

void	philo_routine(t_philo *philo);
void	*dead_check(void *ptr);
void	*eat_check(void *ptr);

#endif