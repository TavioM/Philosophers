/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocmarout <ocmarout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/20 16:40:04 by ocmarout          #+#    #+#             */
/*   Updated: 2021/12/07 15:27:42 by ocmarout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print(t_args *args, t_philo *philo, char *str)
{
	pthread_mutex_lock(&args->write.mutex);
	if ((str[0] == 'd' && !args->death_count.data)
		|| (args->death_count.data && args->nb_philo == 1 && str[0] == 'd'))
	{
		printf("%ld %d %s\n", gettime() - args->start_time, philo->id, str);
		(args->death_count.data)++;
		pthread_mutex_unlock(&args->write.mutex);
		return ;
	}
	if (!args->death_count.data)
		printf("%ld %d %s\n", gettime() - args->start_time, philo->id, str);
	pthread_mutex_unlock(&args->write.mutex);
}

void	*routine(void *data)
{
	int		i;
	t_args	*args;
	t_philo	*philo;

	i = 0;
	philo = (t_philo *)data;
	args = philo->args;
	philo->time_of_death = gettime() + args->time_to_die;
	my_usleep((args->time_to_eat / 2) * !(philo->id % 2));
	while (!args->death_count.data && i++ < args->max_meals)
	{
		pthread_mutex_lock(&args->forks[philo->id - 1].mutex);
		print(args, philo, "has taken a fork");
		pthread_mutex_lock(&args->forks[philo->id % args->nb_philo].mutex);
		print(args, philo, "has taken a fork");
		print(args, philo, "is eating");
		philo->time_of_death = gettime() + args->time_to_die;
		my_usleep(args->time_to_eat);
		pthread_mutex_unlock(&args->forks[philo->id - 1].mutex);
		pthread_mutex_unlock(&args->forks[philo->id % args->nb_philo].mutex);
		print(args, philo, "is sleeping");
		my_usleep(args->time_to_sleep);
		print(args, philo, "is thinking");
	}
	return ((void *)(philo->time_of_death = 0));
}

t_philo	*create_philos(t_args *args)
{
	int		i;
	t_philo	*philo;

	i = 0;
	philo = malloc(sizeof(t_philo) * args->nb_philo);
	args->forks = malloc(sizeof(t_mutex) * args->nb_philo);
	while (i < args->nb_philo)
	{
		philo[i].id = i + 1;
		philo[i].args = args;
		pthread_mutex_init(&args->forks[i].mutex, NULL);
		args->forks[i].data = 0;
		i++;
	}
	pthread_mutex_init(&args->write.mutex, NULL);
	args->write.data = 0;
	pthread_mutex_init(&args->death_count.mutex, NULL);
	args->death_count.data = 0;
	i = -1;
	while (++i < args->nb_philo)
		pthread_create(&philo[i].thread, NULL, &routine, &philo[i]);
	return (philo);
}

void	join_philos(t_philo *philo, int nb)
{
	int	i;

	i = 0;
	while (i < nb)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
}

void	check_death(t_args *args, t_philo *philo)
{
	int	i;
	int	end;

	my_usleep(args->time_to_die);
	if (args->nb_philo == 1)
	{
		pthread_mutex_lock(&args->write.mutex);
		args->death_count.data = 1;
		pthread_mutex_unlock(&args->write.mutex);
		pthread_mutex_unlock(&args->forks[0].mutex);
		print(args, &philo[0], "died");
	}
	while (!args->death_count.data)
	{
		i = 0;
		end = 0;
		while (i < args->nb_philo)
		{
			if (philo[i].time_of_death < gettime() && philo[i].time_of_death)
				print(args, &philo[i], "died");
			end += !philo[i++].time_of_death;
		}
		if (end == args->nb_philo)
			return ;
	}
}
