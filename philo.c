/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocmarout <ocmarout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/20 16:40:04 by ocmarout          #+#    #+#             */
/*   Updated: 2021/12/10 11:10:41 by ocmarout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	routine_loop(t_args *args, t_philo *philo)
{
	while (args->nb_philo == 1)
	{
		if (get_mutex(&args->death_count))
			return (1);
		usleep(500);
	}
	pthread_mutex_lock(&args->forks[philo->id % args->nb_philo]);
	print(args, philo, "has taken a fork");
	pthread_mutex_lock(&args->forks[philo->id - 1]);
	print(args, philo, "has taken a fork");
	print(args, philo, "is eating");
	set_mutex(&philo->time_of_death, gettime() + args->time_to_die);
	my_usleep(args->time_to_eat, args);
	print(args, philo, "is sleeping");
	pthread_mutex_unlock(&args->forks[philo->id - 1]);
	pthread_mutex_unlock(&args->forks[philo->id % args->nb_philo]);
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
	pthread_mutex_lock(&args->start);
	pthread_mutex_unlock(&args->start);
	set_mutex(&philo->time_of_death, gettime() + args->time_to_die);
	my_usleep((args->time_to_eat / 2) * !(philo->id % 2), args);
	while (!get_mutex(&args->death_count) && !get_mutex(&args->end_of_sim))
	{
		if (args->max_meals != -1 && i++ == args->max_meals)
			set_mutex(&args->done_eating, get_mutex(&args->done_eating) + 1);
		if (get_mutex(&args->done_eating) == args->nb_philo)
			return (0);
		if (routine_loop(args, philo) == 1)
			return (0);
	}
	return (0);
}

t_philo	*create_philos(t_args *args)
{
	int		i;
	t_philo	*philo;

	i = 0;
	philo = malloc(sizeof(t_philo) * args->nb_philo);
	if (!philo)
		return (0);
	args->forks = malloc(sizeof(t_mutex) * args->nb_philo);
	if (!args->forks)
	{
		free(philo);
		return (0);
	}
	pthread_mutex_lock(&args->start);
	while (i < args->nb_philo)
	{
		philo[i].id = i + 1;
		philo[i].args = args;
		pthread_mutex_init(&philo[i].time_of_death.mutex, NULL);
		pthread_create(&philo[i].thread, NULL, &routine, &philo[i]);
		pthread_mutex_init(&args->forks[i++], NULL);
	}
	args->start_time = gettime();
	pthread_mutex_unlock(&args->start);
	return (philo);
}

void	check_death(t_args *args, t_philo *philo)
{
	int	i;

	my_usleep(args->time_to_die, args);
	while (!get_mutex(&args->death_count) && !get_mutex(&args->end_of_sim))
	{
		i = 0;
		while (i < args->nb_philo)
		{
			if (get_mutex(&philo[i].time_of_death) < gettime())
				print(args, &philo[i], "died");
			i++;
		}
		if (get_mutex(&args->done_eating) == args->nb_philo)
		{
			set_mutex(&args->end_of_sim, 1);
			return ;
		}
		usleep(50);
	}
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
