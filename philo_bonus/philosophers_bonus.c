/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 07:53:02 by kshim             #+#    #+#             */
/*   Updated: 2022/11/28 13:50:24 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "./philosophers.h"

int	ft_phiosophers_start(t_prg *prg, t_philo *philo, t_sveil *surveil)
{
	int	i;

	i = 0;
	surveil->start_time = ft_set_now_micro_s();
	while (i < surveil->philo_num)
	{
		philo->number = i + 1;
		prg->pid_array[i] = fork();
		if (prg->pid_array[i] == -1)
		{
			// fork_예외처리
		}
		else if (prg->pid_array[i] != 0)
		{
			ft_philo_routine(philo, surveil);
			break ;
		}
		i++;
	}
	pthread_create(&(surveil->surveil_napkin), 0,
		(void *)ft_surveil_napkin, (void *)prg);
	i = 0;
	while (i < surveil->philo_num)
	{
		sem_post(surveil->ipc_sems->start_eat);
		i++;
	}
	pthread_create(&(surveil->surveil_done_eat), 0,
		(void *)ft_surveil_end_philo_done_eat, (void *)surveil);
	return (0);
}

int	ft_philo_routine(t_philo *philo, t_sveil *surveil)
{
	pthread_create(&(surveil->surveil_end), 0,
		(void *)surveil_end, (void *)philo);
	if (philo->number == surveil->philo_num)
		philo->napkin = surveil->napkin_last;
	else if (philo->number % 2 == 1)
		philo->napkin = surveil->napkin_odd;
	else if (philo->number % 2 == 0)
		philo->napkin = surveil->napkin_even;
	sem_wait(surveil->ipc_sems->start_eat);
	while (1)
	{
		if (ft_philo_eat(philo, surveil) != 0)
		{
			sem_post(surveil->ipc_sems->forks);
			sem_post(philo->napkin);
			break ;
		}
		if (ft_philo_sleep_think(philo, surveil) != 0)
			break ;
		sem_wait(surveil->ipc_sems->done);
		if (surveil->stop == 1)
		{
			sem_post(surveil->ipc_sems->done);
			break ;
		}
		sem_post(surveil->ipc_sems->done);
	}
	pthread_join(surveil->surveil_end, 0);
	return (1);
}

int	ft_philo_eat(t_philo *philo, t_sveil *surveil)
{
	sem_wait(philo->napkin);
	sem_wait(surveil->ipc_sems->forks);
	if (ft_print_with_mutex(philo, surveil, "has taken a fork") != 0)
		return (1);
	sem_wait(surveil->ipc_sems->forks);
	if (ft_print_with_mutex(philo, surveil, "has taken a fork") != 0
		|| ft_print_with_mutex(philo, surveil, "is eating") != 0)
	{
		sem_post(surveil->ipc_sems->forks);
		return (1);
	}
	sem_wait(surveil->ipc_sems->last_eat);
	philo->last_eat_time = ft_set_timestamp(philo);
	sem_post(surveil->ipc_sems->last_eat);
	ft_philo_after_eat(philo, surveil);
	ft_usleep(surveil->time_to_eat * 1000);
	sem_post(surveil->ipc_sems->forks);
	sem_post(surveil->ipc_sems->forks);
	sem_post(philo->napkin);
	return (0);
}

// 식사량 보고 관련 동작 다시 한 번 생각해보자. 뮤텍스 사용 불가능하니 고민 필요.
int	ft_philo_after_eat(t_philo *philo, t_sveil *surveil)
{
	philo->number_of_eat++;
	if (philo->number_of_eat == surveil->number_to_eat)
		sem_post(surveil->ipc_sems->philo_done_eat);
	return (0);
}

int	ft_philo_sleep_think(t_philo *philo, t_sveil *surveil)
{
	sem_wait(surveil->ipc_sems->done);
	if (surveil->stop != 1)
	{
		sem_post(surveil->ipc_sems->done);
		if (ft_print_with_mutex(philo, surveil, "is sleeping") != 0)
			return (1);
		ft_usleep(surveil->time_to_sleep * 1000);
		if (ft_print_with_mutex(philo, surveil, "is thinking") != 0)
			return (1);
	}
	sem_post(surveil->ipc_sems->done);
	return (0);
}
