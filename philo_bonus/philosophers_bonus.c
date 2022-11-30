/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 07:53:02 by kshim             #+#    #+#             */
/*   Updated: 2022/11/30 19:01:51 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

#include "./philosophers_bonus.h"

int	ft_phiosophers_start(t_prg *prg, t_philo *philo, t_sveil *surveil)
{
	int	i;

	i = 0;
	ft_fork_philo(philo, surveil, i);
	ft_fork_napkin_done_eat(surveil);
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
	exit(0);
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
	philo->number_of_eat++;
	if (philo->number_of_eat == surveil->number_to_eat)
		sem_post(surveil->ipc_sems->philo_done_eat);
	sem_post(surveil->ipc_sems->last_eat);
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
