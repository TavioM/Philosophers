/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocmarout <ocmarout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 13:12:51 by ocmarout          #+#    #+#             */
/*   Updated: 2021/12/07 18:01:52 by ocmarout         ###   ########.fr       */
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

void	my_usleep(int delay, t_args *args)
{
	long	stop_time;

	stop_time = gettime() + delay;
	while (gettime() < stop_time && !args->death_count.data)
		usleep(50);
}
