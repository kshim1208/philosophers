/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 07:53:02 by kshim             #+#    #+#             */
/*   Updated: 2022/11/22 17:23:24 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "../include/philosophers.h"

int	ft_phiosophers_start(t_prg *prg, t_philo *philo_arr, t_sveil *surveil)
{
	int			i;

	i = 0;
	surveil -> start_time = ft_set_now_ms();
	pthread_create(&(surveil -> surveil_eat), 0,
		(void *)ft_surveil_eat, (void *)prg);
	if (surveil -> philo_num == 1)
		pthread_create(&(philo_arr[0].tid), 0,
			(void *)ft_philo_routine_only_one, (void *)&(philo_arr[0]));
	else
	{
		while (i < prg -> surveil -> philo_num)
		{
			pthread_create(&(philo_arr[i].tid), 0,
				(void *)ft_philo_routine, (void *)&(philo_arr[i]));
			i++;
		}
	}
	ft_surveil_end(philo_arr, surveil);
	ft_finish_philosophers(prg);
	return (0);
}

int	ft_philo_routine_only_one(t_philo *philo)
{
	t_sveil	*surveil;

	surveil = philo -> surveil;
	while (1)
	{
		pthread_mutex_lock(philo -> napkin);
		pthread_mutex_lock(philo -> first_fork);
		ft_print_with_mutex(philo, surveil, "has taken a fork");
		if (ft_usleep((surveil -> time_to_die * 1000) + 100) != 0)
		{
			pthread_mutex_unlock(philo -> first_fork);
			pthread_mutex_unlock(philo -> napkin);
			return (1);
		}
		pthread_mutex_unlock(philo -> first_fork);
		pthread_mutex_lock(surveil -> done);
		if (surveil -> stop == 1)
		{
			pthread_mutex_unlock(surveil -> done);
			pthread_mutex_unlock(philo -> napkin);
			break ;
		}
	}
	return (0);
}

int	ft_philo_routine(t_philo *philo)
{
	t_sveil	*surveil;
	int		ret;

	surveil = philo -> surveil;
	ret = 1;
	while (1)
	{
		if (ft_philo_eat(philo, surveil) != 0)
			break ;
		if (ft_philo_end_or_wait(philo, surveil) != 0)
			break ;
		pthread_mutex_lock(surveil -> done);
		philo -> mutex_lock_check[E_DONE] = 1;
		if (surveil -> stop == 1)
		{
			ret = 0;
			pthread_mutex_unlock(surveil -> done);
			philo -> mutex_lock_check[E_DONE] = 0;
			break ;
		}
		pthread_mutex_unlock(surveil -> done);
		philo -> mutex_lock_check[E_DONE] = 0;
	}
	ft_philo_mutex_unlock(philo, surveil);
	return (ret);
}

int	ft_philo_eat(t_philo *philo, t_sveil *surveil)
{
	pthread_mutex_lock(philo -> napkin);
	pthread_mutex_lock(philo -> first_fork);
	philo -> mutex_lock_check[E_FIRST_FORK] = 1;
	if (ft_print_with_mutex(philo, surveil, "has taken a fork") != 0)
		return (1);
	pthread_mutex_lock(philo -> second_fork);
	philo -> mutex_lock_check[E_SECOND_FORK] = 1;
	if (ft_print_with_mutex(philo, surveil, "has taken a fork") != 0
		|| ft_print_with_mutex(philo, surveil, "is eating") != 0)
		return (1);
	pthread_mutex_lock(philo -> last_eat);
	philo -> last_eat_time = ft_set_timestamp(philo);
	pthread_mutex_unlock(philo -> last_eat);
	ft_usleep(surveil -> time_to_eat * 1000);
	pthread_mutex_unlock(philo -> second_fork);
	philo -> mutex_lock_check[E_SECOND_FORK] = 0;
	pthread_mutex_unlock(philo -> first_fork);
	philo -> mutex_lock_check[E_FIRST_FORK] = 0;
	philo -> number_of_eat++;
	pthread_mutex_unlock(philo -> napkin);
	return (0);
}

int	ft_philo_end_or_wait(t_philo *philo, t_sveil *surveil)
{
	if (philo -> number_of_eat == surveil -> number_to_eat)
	{
		pthread_mutex_lock(surveil -> done);
		surveil -> philo_done_eat++;
		pthread_mutex_unlock(surveil -> done);
	}
	else
	{
		if (ft_print_with_mutex(philo, surveil, "is sleeping") != 0)
			return (1);
		ft_usleep(surveil -> time_to_sleep * 1000);
		if (ft_print_with_mutex(philo, surveil, "is thinking") != 0)
			return (1);
	}
	return (0);
}
