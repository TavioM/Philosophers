/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocmarout <ocmarout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 15:21:40 by ocmarout          #+#    #+#             */
/*   Updated: 2021/12/06 19:02:28 by ocmarout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
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
	int			data;
	t_args		*args;
	pthread_t	thread;
}				t_philo;

#endif

/*
# include <unistd.h>
# include <stdlib.h>
# include <string.h>

typedef struct s_mutex
{
	int				value;
	pthread_mutex_t	lock;
}					t_mutex;

typedef struct s_time
{
	int				sleep;
	int				die;
	int				eat;
	int				start;
}					t_time;

typedef struct s_philo
{
	int				nb;
	int				nb_eat;
	int				last_meal;
	pthread_t		die_thread;
	pthread_t		thread;
}					t_philo;

typedef struct s_state
{
	int				nb_philo;
	int				max_eat;
	t_mutex			output;
	t_mutex			philos_dead;
	t_time			time;
	t_philo			*philos;
	pthread_mutex_t	*forks;
}					t_state;

typedef struct s_args
{
	t_state			*state;
	t_philo			*philo;
}					t_args;

int					ft_isnumber(const char *s);
int					ft_atoi(const char *str);
int					read_mutex(t_mutex *mutex);
void				write_mutex(t_mutex *mutex, int value);
int					gettime(void);
void				ft_usleep(int ms);
void				do_action(t_state *state, t_philo *philo, t_action action,
						int time);
void				*routine(void *args);
t_args				*create_philos(t_state *state);
void				join_philos(t_state *state);
void				exit_philos(t_state *state, t_args *args);
*/
