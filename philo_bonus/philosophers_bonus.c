/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 07:53:02 by kshim             #+#    #+#             */
/*   Updated: 2022/11/29 14:03:32 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

#include "./philosophers_bonus.h"

#include <stdio.h>

int	ft_phiosophers_start(t_prg *prg, t_philo *philo, t_sveil *surveil)
{
	int	i;

	i = 0;
	surveil->start_time = ft_set_now_micro_s();
	while (i < surveil->philo_num)
	{
		philo->number = i + 1;
		surveil->pid_array[i] = fork();
		if (surveil->pid_array[i] == -1)
		{
			//ft_kill_pid_array(surveil);
			ft_exit_after_unlink_sem(surveil);
		}
		else if (surveil->pid_array[i] == 0)
		{
			ft_philo_routine(philo, surveil);
			exit (0);
		}
		i++;
	}
	surveil->surveil_napkin = fork();
	if (surveil->surveil_napkin == 0)
	{
		ft_surveil_napkin(surveil);
		exit (0);
	}
	else if (surveil->surveil_napkin < 0)
	{
		//ft_kill_pid_array(surveil);
		ft_exit_after_unlink_sem(surveil);
	}
	if (surveil->number_to_eat > 0)
	{
		surveil->surveil_done_eat = fork();
		if (surveil->surveil_done_eat == 0)
		{
			ft_surveil_done_eat(surveil);
			exit (0);
		}
		else if (surveil->surveil_done_eat < 0)
		{
			//ft_kill_pid_array(surveil);
			//ft_kill_napkin(surveil);
			ft_exit_after_unlink_sem(surveil);
		}
	}
	i = 0;
	while (i < surveil->philo_num)
	{
		sem_post(surveil->ipc_sems->start_eat);
		i++;
	}
	ft_finish_philosophers(prg, surveil);
	return (0);
}

int	ft_philo_routine(t_philo *philo, t_sveil *surveil)
{
	pthread_create(&(philo->surveil_end), 0,
		(void *)ft_surveil_end, (void *)philo);
	if ((philo->number == surveil->philo_num) && (philo->number % 2 == 1))
		philo->napkin = surveil->ipc_sems->napkin_last;
	else if (philo->number % 2 == 0)
		philo->napkin = surveil->ipc_sems->napkin_even;
	else
		philo->napkin = surveil->ipc_sems->napkin_odd;
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
	pthread_join(philo->surveil_end, 0);
	return (1);
}

int	ft_philo_eat(t_philo *philo, t_sveil *surveil)
{
	sem_wait(philo->napkin);
	sem_wait(surveil->ipc_sems->forks);
	if (ft_print_with_sema(philo, surveil, "has taken a fork") != 0)
		return (1);
	sem_wait(surveil->ipc_sems->forks);
	if (ft_print_with_sema(philo, surveil, "has taken a fork") != 0
		|| ft_print_with_sema(philo, surveil, "is eating") != 0)
	{
		sem_post(surveil->ipc_sems->forks);
		return (1);
	}
	ft_philo_after_eat(philo, surveil);
	ft_usleep(surveil->time_to_eat * 1000);
	sem_post(surveil->ipc_sems->forks);
	sem_post(surveil->ipc_sems->forks);
	sem_post(philo->napkin);
	return (0);
}

int	ft_philo_after_eat(t_philo *philo, t_sveil *surveil)
{
	sem_wait(surveil->ipc_sems->last_eat);
	philo->last_eat_time = ft_set_timestamp(philo);
	sem_post(surveil->ipc_sems->last_eat);
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
		if (ft_print_with_sema(philo, surveil, "is sleeping") != 0)
			return (1);
		ft_usleep(surveil->time_to_sleep * 1000);
		if (ft_print_with_sema(philo, surveil, "is thinking") != 0)
			return (1);
	}
	sem_post(surveil->ipc_sems->done);
	return (0);
}
