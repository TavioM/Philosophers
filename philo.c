/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocmarout <ocmarout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/20 16:40:04 by ocmarout          #+#    #+#             */
/*   Updated: 2021/12/06 19:31:19 by ocmarout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
# include <stdlib.h>
# include <unistd.h>

long	gettime(void)
{
	struct timeval  t;

	if (gettimeofday(&t, NULL) == -1)
		return (0);
	return (t.tv_sec * 1000 + t.tv_usec / 1000);
}

void	my_usleep(int delay, t_philo *p)
{
	long	stop_time;

	(void)p;
	stop_time = gettime() + delay;
//	printf("stop_time : %ld\n", stop_time);
//	printf("gettime : %ld\n", gettime());
//	printf("delay : %d\n", delay);
	while (gettime() < stop_time)
	{
	//	printf("id = %d, d = %d\n", p->id, delay);
		usleep(50);
	}
}

void	*routine(void *data)
{
	int		i;
	t_args	*args;
	t_philo	*philo;

	i = 0;
	philo = (t_philo *)data;
	args = philo->args;
	if (!(philo->id % 2))
		my_usleep(args->time_to_eat / 2, philo);
//	if (philo->id == 1)
//	{
//		printf("time to sleep %d\n", args->time_to_sleep);
//	}
	while (!args->death_count.data)
	{
//		printf("coucou 1 id : %d\n", philo->id);
		pthread_mutex_lock(&args->forks[philo->id - 1].mutex);
		printf("%ld %d has taken a fork\n", gettime() - args->start_time, philo->id);
		pthread_mutex_lock(&args->forks[philo->id % args->nb_philo].mutex);
	printf("%ld %d has taken a fork (2)\n", gettime() - args->start_time, philo->id);
		printf("%ld %d is eating\n", gettime() - args->start_time, philo->id);
		my_usleep(args->time_to_eat, philo);
		pthread_mutex_unlock(&args->forks[philo->id - 1].mutex);
		pthread_mutex_unlock(&args->forks[philo->id % args->nb_philo].mutex);
		printf("%ld %d is sleeping\n", gettime() - args->start_time, philo->id);
//		printf("%ld\n", gettime());
		my_usleep(args->time_to_sleep, philo);
//		printf("%ld\n", gettime());
		i++;
//		printf("coucou 2 id : %d\n", philo->id);
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
		philo[i].data = 2;
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
	usleep(100000000);
	args.death_count.data = 1;
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
