/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 07:53:02 by kshim             #+#    #+#             */
/*   Updated: 2022/11/22 12:29:34 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>

#include "../include/philosophers.h"

#include <stdio.h>

int	ft_phiosophers_start(t_prg *prg, t_philo *philo_arr, t_sveil *surveil)
{
	int			i;

	i = 0;
	surveil -> start_time = ft_set_now_ms();
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
		pthread_mutex_lock(philo -> first_fork);
		ft_print_with_mutex(philo, surveil, "has taken a fork");
		if (ft_usleep(surveil -> time_to_die * 1000) != 0)
			return (1);
		pthread_mutex_unlock(philo -> first_fork);
		pthread_mutex_lock(surveil -> done);
		if (surveil -> stop == 1)
			break ;
		pthread_mutex_unlock(surveil -> done);
	}
	return (0);
}

int	ft_philo_routine(t_philo *philo)
{
	t_sveil	*surveil;
	int		ret;

	surveil = philo -> surveil;
	if (philo -> number % 2 == 0)
		ft_usleep((surveil -> time_to_eat / 10.0) * 1000.0);
	while (1)
	{
		if (ft_philo_eat(philo, surveil) != 0)
			break ;
		if (philo -> number_of_eat == surveil -> number_to_eat)
		{
			pthread_mutex_lock(surveil -> done);
			surveil -> philo_done_eat++;
			pthread_mutex_unlock(surveil -> done);
			break ;
		}
		else
		{
			if (ft_print_with_mutex(philo, surveil, "is sleeping") != 0)
				break ;
			ft_usleep(surveil -> time_to_sleep * 1000);
			if (ft_print_with_mutex(philo, surveil, "is thinking") != 0)
				break ;
		}
		pthread_mutex_lock(surveil -> done);
		philo -> mutex_lock_check[E_DONE] = 1;
		if (surveil -> stop == 1)
			break ;
		pthread_mutex_unlock(surveil -> done);
		philo -> mutex_lock_check[E_DONE] = 0;
	}
	ft_philo_mutex_unlock(philo, surveil);
	return (0);
}

int	ft_philo_eat(t_philo *philo, t_sveil *surveil)
{
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
	return (0);
}

void	ft_philo_mutex_unlock(t_philo *philo, t_sveil *surveil)
{
	if (philo -> mutex_lock_check[E_FIRST_FORK] == 1)
		pthread_mutex_unlock(philo -> first_fork);
	if (philo -> mutex_lock_check[E_SECOND_FORK] == 1)
		pthread_mutex_unlock(philo -> second_fork);
	if (philo -> mutex_lock_check[E_PRINT] == 1)
		pthread_mutex_unlock(surveil -> print);
	if (philo -> mutex_lock_check[E_DONE] == 1)
		pthread_mutex_unlock(surveil -> done);
	return ;
}
// pthread 해제, mutex 해제, prg 디렉토리 해제 함수를 분리 
	// intilize 단계에서 실패했을 때도 일부 사용할 수 있도록

	// 1개만 있을 때 join or detach 필요함
int	ft_finish_philosophers(t_prg *prg)
{
	int	i;

	i = 0;
	while (i < prg -> surveil -> philo_num)
	{
		pthread_join(prg -> philo_arr[i].tid, 0);
		i++;
	}
	pthread_mutex_unlock(prg -> surveil -> print);
	i = 0;
	while (i < prg -> surveil -> philo_num)
	{
		pthread_mutex_destroy(&(prg -> fork_arr[i]));
		pthread_mutex_destroy(&(prg -> last_eat_arr[i]));
		i++;
	}
	if (prg -> fork_arr != 0)
		free(prg -> fork_arr);
	if (prg -> last_eat_arr != 0)
		free(prg -> last_eat_arr);
	if (prg -> surveil != 0)
	{
		if (prg -> surveil -> print != 0)
		{
			pthread_mutex_destroy(prg -> surveil -> print);
			free(prg -> surveil -> print);
		}
		if (prg -> surveil -> done != 0)
		{
			pthread_mutex_destroy(prg -> surveil -> done);
			free(prg -> surveil -> done);
		}
		free(prg -> surveil);
	}
	if (prg -> philo_arr != 0)
		free(prg -> philo_arr);
	return (0);
}
