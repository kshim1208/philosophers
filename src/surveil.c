/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   surveil.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 10:13:09 by kshim             #+#    #+#             */
/*   Updated: 2022/11/22 15:23:05 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/philosophers.h"

int	ft_surveil_end(t_philo *philo_arr, t_sveil *surveil)
{
	int			i;

	while (1)
	{
		i = 0;
		while (i < surveil -> philo_num)
		{
			pthread_mutex_lock(philo_arr[i].last_eat);
			if (ft_set_time_after_last_eat(
					&(philo_arr[i])) >= surveil -> time_to_die)
			{
				pthread_mutex_lock(surveil -> done);
				surveil -> stop = 1;
				pthread_mutex_unlock(surveil -> done);
				pthread_mutex_lock(surveil -> print);
				if (printf("%llu %d died\n", ft_set_timestamp(&(philo_arr[i])),
						philo_arr[i].number) == -1)
					return (1);
				break ;
			}
			pthread_mutex_unlock(philo_arr[i].last_eat);
			i++;
		}
		if (surveil -> stop != 1)
		{
			pthread_mutex_lock(surveil -> done);
			if (surveil -> philo_done_eat == surveil -> philo_num)
			{
				surveil -> stop = 1;
				pthread_mutex_lock(surveil -> print);
				if (printf("done to eat\n") == -1)
					return (1);
			}
			pthread_mutex_unlock(surveil -> done);
		}
		if (surveil -> stop == 1)
			break ;
	}
	return (0);
}

int	ft_surveil_eat(t_prg *prg)
{
	int		type;
	int		i;
	t_philo	*philo_arr;
	t_sveil	*surveil;

	i = 0;
	type = E_ODD;
	philo_arr = prg -> philo_arr;
	surveil = prg -> surveil;
	while (i < surveil -> philo_num)
	{
		pthread_mutex_lock(philo_arr[i].napkin);
		i++;
	}
	while (1)
	{
		if (type == E_LAST)
		{
			pthread_mutex_unlock(philo_arr[surveil -> philo_num - 1].napkin);
			ft_usleep(surveil -> time_to_eat * 1000);
			pthread_mutex_lock(philo_arr[surveil -> philo_num - 1].napkin);
		}
		else
		{
			i = 0;
			while (i < surveil -> philo_num)
			{
				pthread_mutex_unlock(philo_arr[i + type].napkin);
				i = i + 2;
			}
			// '절반' 저장해두고 잘 먹었나 체크까지.
			ft_usleep(surveil -> time_to_eat * 1000);
			i = 0;
			while (i < surveil -> philo_num)
			{
				pthread_mutex_lock(philo_arr[i + type].napkin);
				i = i + 2;
			}
		}
		if (type == E_LAST)
			type = E_ODD;
		else if (type == E_ODD)
			type = E_EVEN;
		else if (type == E_EVEN)
			type = E_LAST;
		pthread_mutex_unlock(surveil -> done);
		if (surveil -> stop == 1)
		{
			pthread_mutex_lock(surveil -> done);
			break ;
		}
		pthread_mutex_lock(surveil -> done);
	}
	return (0);
}
