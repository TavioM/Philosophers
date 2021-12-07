/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocmarout <ocmarout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/20 16:40:04 by ocmarout          #+#    #+#             */
/*   Updated: 2021/12/07 20:41:15 by ocmarout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	routine_loop(t_args *args, t_philo *philo)
{
	pthread_mutex_lock(&args->forks[philo->id - 1]);
	print(args, philo, "has taken a fork");
	while (args->nb_philo == 1)
	{
		pthread_mutex_lock(&args->death_count.mutex);
		if (args->death_count.data)
			return (1);
		usleep(500);
		pthread_mutex_unlock(&args->death_count.mutex);
	}
	pthread_mutex_lock(&args->forks[philo->id % args->nb_philo]);
	print(args, philo, "has taken a fork");
	print(args, philo, "is eating");
	pthread_mutex_lock(&args->read);
	philo->time_of_death = gettime() + args->time_to_die;
	pthread_mutex_unlock(&args->read);
	my_usleep(args->time_to_eat, args);
	pthread_mutex_unlock(&args->forks[philo->id - 1]);
	pthread_mutex_unlock(&args->forks[philo->id % args->nb_philo]);
	print(args, philo, "is sleeping");
	my_usleep(args->time_to_sleep, args);
	print(args, philo, "is thinking");
	return (0);
}

void	*routine(void *data)
{
	int		i;
	t_args	*args;
	t_philo	*philo;

	i = 0;
	philo = (t_philo *)data;
	args = philo->args;
	pthread_mutex_lock(&args->death_count.mutex);
	pthread_mutex_unlock(&args->death_count.mutex);
	pthread_mutex_lock(&args->read);
	philo->time_of_death = gettime() + args->time_to_die;
	pthread_mutex_unlock(&args->read);
	my_usleep((args->time_to_eat / 2) * !(philo->id % 2), args);
	while (!args->death_count.data && i++ < args->max_meals)
	{
		if (routine_loop(args, philo) == 1)
			return (0);
	}
	pthread_mutex_lock(&args->read);
	philo->time_of_death = 0;
	pthread_mutex_unlock(&args->read);
	return (0);
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
		pthread_mutex_init(&args->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&args->read, NULL);
	pthread_mutex_init(&args->write, NULL);
	pthread_mutex_init(&args->death_count.mutex, NULL);
	pthread_mutex_lock(&args->death_count.mutex);
	args->death_count.data = 0;
	i = -1;
	while (++i < args->nb_philo)
		pthread_create(&philo[i].thread, NULL, &routine, &philo[i]);
	args->start_time = gettime();
	pthread_mutex_unlock(&args->death_count.mutex);
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

	my_usleep(args->time_to_die, args);
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
		usleep(500);
	}
}
