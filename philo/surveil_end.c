/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   surveil_end.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 16:41:47 by kshim             #+#    #+#             */
/*   Updated: 2022/11/30 19:12:09 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

#include "./philosophers.h"

int	ft_surveil_end(t_prg *prg)
{
	t_sveil		*surveil;

	surveil = prg->surveil;
	while (1)
	{
		pthread_mutex_lock(surveil->done);
		if (surveil->stop != 1)
		{
			pthread_mutex_unlock(surveil->done);
			i = 0;
			if (ft_surveil_end_last_eat(prg -> philo_arr, surveil) != 0)
				return (1);
		}
		else
		{
			pthread_mutex_unlock(surveil->done);
			break ;
		}
	}
	return (0);
}

int	ft_surveil_end_last_eat(t_philo *philo_arr, t_sveil *surveil)
{
	int	i;

	i = 0;
	while (i < surveil->philo_num)
	{
		pthread_mutex_lock(philo_arr[i].last_eat);
		if (ft_set_time_after_last_eat(
				&(philo_arr[i])) >= (uint64_t)surveil->time_to_die_micro)
		{
			pthread_mutex_lock(surveil->print);
			pthread_mutex_unlock(philo_arr[i].last_eat);
			pthread_mutex_lock(surveil->done);
			surveil->stop = 1;
			pthread_mutex_unlock(surveil->done);
			if (printf("%llu %d died\n",
					ft_set_timestamp(&(philo_arr[i])) / 1000,
					philo_arr[i].number) == -1)
				return (1);
			pthread_mutex_unlock(surveil->print);
			return (0);
		}
		pthread_mutex_unlock(philo_arr[i].last_eat);
		i++;
	}
	return (0);
}
