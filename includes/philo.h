/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocmarout <ocmarout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 15:21:40 by ocmarout          #+#    #+#             */
/*   Updated: 2021/12/07 13:15:33 by ocmarout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_mutex
{
	int				data;
	pthread_mutex_t	mutex;
}				t_mutex;

typedef struct s_args
{
	int		nb_philo;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		max_meals;
	long	start_time;
	t_mutex	*forks;
	t_mutex	write;
	t_mutex	death_count;
}				t_args;

typedef struct s_philo
{
	int			id;
	long		time_of_death;
	t_args		*args;
	pthread_t	thread;
}				t_philo;

long	gettime(void);
void	my_usleep(int delay);
int		ft_atoi(char const *str);
void	trim_spaces(char *s);
char	*ft_itoa(int n);
int		ft_strcmp(char const *s1, char const *s2);
t_philo	*create_philos(t_args *args);
void	check_death(t_args *args, t_philo *philo);
void	join_philos(t_philo *philo, int nb);
int		ft_atoi(char const *str);

#endif
