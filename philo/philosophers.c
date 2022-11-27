/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 07:53:02 by kshim             #+#    #+#             */
/*   Updated: 2022/11/28 08:18:58 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "./philosophers.h"

int	ft_phiosophers_start(t_prg *prg, t_philo *philo_arr, t_sveil *surveil)
{
	int			i;

	i = 0;
	surveil->start_time = ft_set_now_ms();
	pthread_create(&(surveil->surveil_end), 0,
		(void *)ft_surveil_end, (void *)prg);
	ft_surveil_eat_set_napkin(surveil, E_LOCK);
	if (surveil->philo_num == 1)
		pthread_create(&(philo_arr[0].tid), 0,
			(void *)ft_philo_routine_only_one, (void *)&(philo_arr[0]));
	else
	{
		while (i < prg->surveil->philo_num)
		{
			pthread_create(&(philo_arr[i].tid), 0,
				(void *)ft_philo_routine, (void *)&(philo_arr[i]));
			i++;
		}
	}
	ft_surveil_eat(philo_arr, surveil);
	ft_finish_philosophers(prg);
	return (0);
}

int	ft_philo_routine(t_philo *philo)
{
	t_sveil	*surveil;

	surveil = philo->surveil;
	while (1)
	{
		if (ft_philo_eat(philo, surveil) != 0)
		{
			*(philo->fst_fork_state) = E_DROP;
			pthread_mutex_unlock(philo->first_fork);
			pthread_mutex_unlock(philo->napkin);
			return (1);
		}
		if (ft_philo_sleep_think(philo, surveil) != 0)
			return (1);
		pthread_mutex_lock(surveil->done);
		if (surveil->stop == 1)
		{
			pthread_mutex_unlock(surveil->done);
			return (0);
		}
		pthread_mutex_unlock(surveil->done);
	}
	return (1);
}

int	ft_philo_eat(t_philo *philo, t_sveil *surveil)
{
	pthread_mutex_lock(philo->napkin);
	pthread_mutex_lock(philo->first_fork);
	*(philo->fst_fork_state) = E_HOLD;
	if (ft_print_with_mutex(philo, surveil, "has taken a fork") != 0)
		return (1);
	pthread_mutex_lock(philo->second_fork);
	*(philo->sec_fork_state) = E_HOLD;
	if (ft_print_with_mutex(philo, surveil, "has taken a fork") != 0
		|| ft_print_with_mutex(philo, surveil, "is eating") != 0)
	{
		*(philo->sec_fork_state) = E_DROP;
		pthread_mutex_unlock(philo->second_fork);
		return (1);
	}
	pthread_mutex_lock(philo->last_eat);
	philo->last_eat_time = ft_set_timestamp(philo);
	pthread_mutex_unlock(philo->last_eat);
	ft_philo_after_eat(philo, surveil);
	ft_usleep(surveil->time_to_eat * 1000);
	*(philo->sec_fork_state) = E_DROP;
	pthread_mutex_unlock(philo->second_fork);
	*(philo->fst_fork_state) = E_DROP;
	pthread_mutex_unlock(philo->first_fork);
	pthread_mutex_unlock(philo->napkin);
	return (0);
}

int	ft_philo_after_eat(t_philo *philo, t_sveil *surveil)
{
	pthread_mutex_lock(surveil->done);
	philo->number_of_eat++;
	if (philo->number_of_eat == surveil->number_to_eat)
	{
		surveil->philo_done_eat++;
		if (surveil->philo_done_eat == surveil->philo_num)
		{
			surveil->stop = 1;
			pthread_mutex_unlock(surveil->done);
			return (1);
		}
		pthread_mutex_unlock(surveil->done);
	}
	else
		pthread_mutex_unlock(surveil->done);
	return (0);
}

int	ft_philo_sleep_think(t_philo *philo, t_sveil *surveil)
{
	pthread_mutex_lock(surveil->done);
	if (surveil->stop != 1)
	{
		pthread_mutex_unlock(surveil->done);
		if (ft_print_with_mutex(philo, surveil, "is sleeping") != 0)
			return (1);
		ft_usleep(surveil->time_to_sleep * 1000);
		if (ft_print_with_mutex(philo, surveil, "is thinking") != 0)
			return (1);
	}
	pthread_mutex_unlock(surveil->done);
	return (0);
}
