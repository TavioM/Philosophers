/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocmarout <ocmarout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 13:12:51 by ocmarout          #+#    #+#             */
/*   Updated: 2021/12/08 22:36:12 by ocmarout         ###   ########.fr       */
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

void	print(t_args *args, t_philo *philo, char *str)
{
	int	death_count;

	pthread_mutex_lock(&args->write);
	death_count = get_mutex(&args->death_count);
	if (str[0] == 'd' && !death_count)
	{
		printf("%ld %d %s\n", gettime() - args->start_time, philo->id, str);
		set_mutex(&args->death_count, death_count + 1);
		pthread_mutex_unlock(&args->write);
		return ;
	}
	if (!death_count)
		printf("%ld %d %s\n", gettime() - args->start_time, philo->id, str);
	pthread_mutex_unlock(&args->write);
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
