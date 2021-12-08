/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocmarout <ocmarout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 13:12:51 by ocmarout          #+#    #+#             */
/*   Updated: 2021/12/08 16:11:05 by ocmarout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_mutex(t_mutex *mutex, long value)
{
	pthread_mutex_lock(&mutex->mutex);
	mutex->data = value;
	pthread_mutex_unlock(&mutex->mutex);
}

long	get_mutex(t_mutex *mutex)
{
	long	value;

	pthread_mutex_lock(&mutex->mutex);
	value = mutex->data;
	pthread_mutex_unlock(&mutex->mutex);
	return (value);
}

void	update_death(t_args *args, t_philo *philo, long value)
{
	pthread_mutex_lock(&philo->time_of_death.mutex);
	philo->time_of_death.data = value + args->time_to_die;
	pthread_mutex_unlock(&philo->time_of_death.mutex);
}

long	gettime(void)
{
	struct timeval	t;

	if (gettimeofday(&t, NULL) == -1)
		return (0);
	return (t.tv_sec * 1000 + t.tv_usec / 1000);
}

void	my_usleep(int delay, t_args *args)
{
	long	stop_time;
	long	death_count;

	stop_time = gettime() + delay;
	while (gettime() < stop_time)
	{
		usleep(50);
		pthread_mutex_lock(&args->death_count.mutex);
		death_count = args->death_count.data;
		pthread_mutex_unlock(&args->death_count.mutex);
		if (death_count)
			return ;
	}
}
