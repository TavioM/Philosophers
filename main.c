/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocmarout <ocmarout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 12:15:06 by ocmarout          #+#    #+#             */
/*   Updated: 2021/12/07 14:25:24 by ocmarout         ###   ########.fr       */
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
	args->start_time = gettime();
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
		args->max_meals = 2147483647;
	return (0);
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
	if (argc == 6)
		args.max_meals = atoi(argv[5]);
	philo = create_philos(&args);
	check_death(&args, philo);
	join_philos(philo, args.nb_philo);
	while (i < args.nb_philo)
		pthread_mutex_destroy(&args.forks[i++].mutex);
	free(philo);
	free(args.forks);
	return (0);
}
