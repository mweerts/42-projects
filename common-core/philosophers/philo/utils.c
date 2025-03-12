/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 13:42:31 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/12 14:55:03 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi_custom(const char *str)
{
	long	res;
	int		i;

	i = 0;
	res = 0;
	while (str[i] && ((str[i] >= 9 && str[i] <= 13) || str[i] == ' '))
		i++;
	if (str[i] == '-')
		return (-1);
	else if (str[i] == '+')
		i++;
	if (!(str[i] >= '0' && str[i] <= '9'))
		return (-1);
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + str[i] - 48;
		if (res > INT_MAX)
			return (-1);
		i++;
	}
	return ((int)res);
}

int	free_data(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		if (data->philo_arr && data->philo_arr[i].left_fork)
		{
			free(data->philo_arr[i].left_fork);
			data->philo_arr[i].left_fork = NULL;
		}
		if (data->philo_arr && data->philo_arr[i].right_fork)
		{
			free(data->philo_arr[i].right_fork);
			data->philo_arr[i].right_fork = NULL;
		}
		i++;
	}
	if (data->philo_arr)
	{
		free(data->philo_arr);
		data->philo_arr = NULL;
	}
	return (1);
}
