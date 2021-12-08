/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocmarout <ocmarout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 12:15:06 by ocmarout          #+#    #+#             */
/*   Updated: 2021/12/08 16:33:14 by ocmarout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_strcmp(char const *s1, char const *s2)
{
	unsigned int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - s2[i]);
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

int	check_entry(int *nb, char *str)
{
	char	*tmp;

	trim_spaces(str);
	*nb = ft_atoi(str);
	tmp = ft_itoa(*nb);
	if (ft_strcmp(str, tmp) || *nb < 0)
	{
		free(tmp);
		return (1);
	}
	free(tmp);
	return (0);
}

int	setup(int argc, char **argv, t_args *args)
{
	if (argc != 5 && argc != 6)
		return (1);
	if (check_entry(&args->nb_philo, argv[1]))
		return (1);
	if (check_entry(&args->time_to_die, argv[2]))
		return (1);
	if (check_entry(&args->time_to_eat, argv[3]))
		return (1);
	if (check_entry(&args->time_to_sleep, argv[4]))
		return (1);
	if (argc == 6)
	{
		if (check_entry(&args->max_meals, argv[5]))
			return (1);
	}
	else
		args->max_meals = -1;
	pthread_mutex_init(&args->start, NULL);
	pthread_mutex_init(&args->write, NULL);
	args->death_count.data = 0;
	pthread_mutex_init(&args->death_count.mutex, NULL);
	args->end_of_simulation.data = 0;
	pthread_mutex_init(&args->end_of_simulation.mutex, NULL);
	return (0);
}

void	destroyer(t_args *args, t_philo *philo)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&args->start);
	pthread_mutex_destroy(&args->write);
	pthread_mutex_destroy(&args->death_count.mutex);
	pthread_mutex_destroy(&args->end_of_simulation.mutex);
	while (i < args->nb_philo)
		pthread_mutex_destroy(&args->forks[i++]);
	free(args->forks);
	free(philo);
}

int	main(int argc, char **argv)
{
	int		i;
	t_args	args;
	t_philo	*philo;

	i = 0;
	if (setup(argc, argv, &args))
	{
		printf("Parsing Error\n");
		return (-1);
	}
	philo = create_philos(&args);
	check_death(&args, philo);
	join_philos(philo, args.nb_philo);
	destroyer(&args, philo);
	return (0);
}
