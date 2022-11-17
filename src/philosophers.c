/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 07:53:02 by kshim             #+#    #+#             */
/*   Updated: 2022/11/17 13:54:27 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/philosophers.h"

// 스레드 시작을 반복문으로 하는데, 이로 인한 각 스레드의 시작 시간 차이를 최소화할 수는 없을까?
	// 일단 philo_arr[i]의 요소를 가능한 한 먼저 채워두고, time은 반복문 전에 한 번만 함수 호출하여 값을 넣는 식으로 만들었다.
	// 근데 이러면 now를 그 스레드의 시작 시간이라고 볼 수 있을까?
int	ft_phiosophers_start(t_prg *prg, t_philo *philo_arr, t_sveil *surveil)
{
	int			i;

	i = 0;
	while (i < prg -> surveil -> philo_num)
	{
		philo_arr[i].start_time = ft_set_start_time(&(philo_arr[i]));
		pthread_create(&(philo_arr[i].tid), 0,
			(void *)ft_philo_routine, (void *)&(philo_arr[i]));
		i++;
	}
	pthread_create(&(surveil -> surveil_eat), 0,
		(void *)ft_surveil_eat, (void *)surveil);
	ft_surveil_end(philo_arr, surveil);
	ft_finish_philosophers(prg);
	return (1);
}

// ft_set_mili_time 기능 구체화하고, 적절한 구성 생각해보기
	// number_to_eat이랑은 어떻게 동작하게 만들까?
	// 그냥 계속 먹게 두고, surveil에서만 검사할까?
void	ft_philo_routine(t_philo *philo)
{
	t_sveil	*surveil;

	surveil = philo -> surveil;
	if (philo -> number % 2 == 0)
		usleep(1000);
	while (surveil -> stop != 1)
	{
		ft_philo_eat(philo, surveil);
		ft_print_with_mutex(philo, surveil, "is sleeping");
		usleep(surveil -> time_to_sleep * 1000);
		ft_print_with_mutex(philo, surveil, "is thinking");
	}
	return ;
}

void	ft_philo_eat(t_philo *philo, t_sveil *surveil)
{
	pthread_mutex_lock(philo -> napkin);
	pthread_mutex_lock(philo -> first_fork);
	ft_print_with_mutex(philo, surveil, "has taken a fork");
	pthread_mutex_lock(philo -> second_fork);
	philo -> last_eat_time = ft_set_timestamp(philo);
	ft_print_with_mutex(philo, surveil, "has taken a fork");
	ft_print_with_mutex(philo, surveil, "is eating");
	usleep(surveil -> time_to_eat * 1000);
	pthread_mutex_unlock(philo -> second_fork);
	pthread_mutex_unlock(philo -> first_fork);
	pthread_mutex_unlock(philo -> napkin);
	philo -> number_of_eat++;
	return ;
}

void	ft_surveil_eat(t_sveil *surveil)
{
	int	i;

	surveil -> eat_type = E_ODD;
	while (surveil -> stop != 1)
	{
		if (surveil -> eat_type == E_ODD)
		{
			i = 2;
			while (i < surveil -> philo_num)
			{
				pthread_mutex_unlock(&(surveil -> napkin_arr[i]));
				i += 2;
			}
			usleep(surveil -> time_to_eat * 1000);
			i = 2;
			while (i < surveil -> philo_num)
			{
				pthread_mutex_lock(&(surveil -> napkin_arr[i]));
				i += 2;
			}
		}
		else if (surveil -> eat_type == E_EVEN)
		{
			i = 2;
			while (i < surveil -> philo_num)
			{
				pthread_mutex_unlock(&(surveil -> napkin_arr[-1 + i]));
				i += 2;
			}
			usleep(surveil -> time_to_eat * 1000);
			i = 2;
			while (i < surveil -> philo_num)
			{
				pthread_mutex_lock(&(surveil -> napkin_arr[-1 + i]));
				i += 2;
			}
		}
		/*
		else if (surveil -> eat_type == E_LAST)
		{
			pthread_mutex_unlock(&(surveil -> napkin_arr[(surveil -> philo_num - 1)]));
			pthread_mutex_lock(&(surveil -> napkin_arr[(surveil -> philo_num - 1)]));
		}
		if (surveil -> eat_type == E_LAST)
			surveil -> eat_type = E_ODD;
		else
			surveil -> eat_type++;
		*/
		if (surveil -> eat_type == E_ODD)
			surveil -> eat_type = E_EVEN;
		else
			surveil -> eat_type = E_ODD;
	}
	return ;
}

void	ft_surveil_end(t_philo *philo_arr, t_sveil *surveil)
{
	int	i;
	int	done_to_eat;

	while (surveil -> stop != 1)
	{
		i = 0;
		done_to_eat = 0;
		while (i < surveil -> philo_num)
		{
			if (ft_set_time_after_last_eat(&(philo_arr[i])) >= surveil -> time_to_die)
			{
				surveil -> stop = 1;
				ft_print_with_mutex(&(philo_arr[i]), surveil, "died");
				break ;
			}
			else if (philo_arr[i].number_of_eat == surveil -> number_to_eat)
				done_to_eat++;
			i++;
		}
		if (surveil -> stop != 1)
		{
			if (done_to_eat == surveil -> philo_num)
				surveil -> stop = 1;
			pthread_mutex_lock(surveil -> print);
		}
	}
	return ;
}

void	ft_finish_philosophers(t_prg *prg)
{
	if (prg == 0)
		return ;
	if (prg -> surveil != 0)
	{
		if (prg -> surveil -> print != 0)
		{
			pthread_mutex_destroy(prg -> surveil -> print);
			free(prg -> surveil -> print);
		}
		free(prg -> surveil);
	}
	// fork mutex도 destroy 핋요
	if (prg -> fork_arr != 0)
		free(prg -> fork_arr);
	// philo pthread_join 필요
	if (prg -> philo_arr != 0)
		free(prg -> philo_arr);
	return ;
}
