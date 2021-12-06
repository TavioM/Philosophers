/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocmarout <ocmarout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/20 16:40:04 by ocmarout          #+#    #+#             */
/*   Updated: 2021/12/06 21:57:37 by ocmarout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	gettime(void)
{
	struct timeval	t;

	if (gettimeofday(&t, NULL) == -1)
		return (0);
	return (t.tv_sec * 1000 + t.tv_usec / 1000);
}

void	my_usleep(int delay)
{
	long	stop_time;

	stop_time = gettime() + delay;
	while (gettime() < stop_time)
		usleep(50);
}

void	print(t_args *args, t_philo *philo, char *str)
{
	pthread_mutex_lock(&args->write.mutex);
	if (str[0] == 'd' && !args->death_count.data)
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
	if (!(philo->id % 2))
		my_usleep(args->time_to_eat / 2);
	while (!args->death_count.data && i < args->max_meals)
	{
		if (philo->time_of_death < gettime())
		{
			print(args, philo, "died");
			return (0);
		}
		pthread_mutex_lock(&args->forks[philo->id - 1].mutex);
		print(args, philo, "has taken a fork");
		pthread_mutex_lock(&args->forks[philo->id % args->nb_philo].mutex);
		print(args, philo, "has taken a fork (2)");
		print(args, philo, "is eating");
		my_usleep(args->time_to_eat);
		pthread_mutex_unlock(&args->forks[philo->id - 1].mutex);
		pthread_mutex_unlock(&args->forks[philo->id % args->nb_philo].mutex);
		print(args, philo, "is sleeping");
		my_usleep(args->time_to_sleep);
		print(args, philo, "is thinking");
		i++;
	}
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
		printf("join with %d at %lu\n", i, philo[i].thread);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_args	args;
	t_philo	*philo;

	args.start_time = gettime();
	if (argc != 5 && argc != 6)
		return (1);
	args.nb_philo = atoi(argv[1]);
	args.time_to_die = atoi(argv[2]);
	args.time_to_eat = atoi(argv[3]);
	args.time_to_sleep = atoi(argv[4]);
	if (argc == 6)
		args.max_meals = atoi(argv[5]);
	philo = create_philos(&args);
	join_philos(philo, args.nb_philo);
	int i = 0;
	while (i < args.nb_philo)
	{
		pthread_mutex_destroy(&args.forks[i].mutex);
		i++;
	}
	free(philo);
	return (0);
}
