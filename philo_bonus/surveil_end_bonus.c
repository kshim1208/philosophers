/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   surveil_end_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 16:41:47 by kshim             #+#    #+#             */
/*   Updated: 2022/11/30 16:53:45 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "./philosophers_bonus.h"

#include <signal.h>

int	ft_surveil_end(t_philo *philo)
{
	t_sveil		*surveil;

	surveil = philo->surveil;
	while (1)
	{
		if (ft_surveil_end_last_eat(philo, surveil) != 0)
			return (1);
	}
	return (0);
}

int	ft_surveil_end_last_eat(t_philo *philo, t_sveil *surveil)
{
	sem_wait(surveil->ipc_sems->last_eat);
	if (ft_set_time_after_last_eat(
			philo) >= (uint64_t)surveil->time_to_die_micro)
	{
		sem_post(surveil->ipc_sems->last_eat);
		sem_wait(surveil->ipc_sems->done);
		surveil->stop = 1;
		sem_post(surveil->ipc_sems->done);
		sem_wait(surveil->ipc_sems->print);
		if (printf("%llu %d died\n", ft_set_timestamp(philo) / 1000,
				philo->number) == -1)
		{
			sem_post(surveil->ipc_sems->finish);
			exit (1);
		}
		sem_post(surveil->ipc_sems->finish);
		exit(0);
	}
	sem_post(surveil->ipc_sems->done);
	sem_post(surveil->ipc_sems->last_eat);
	return (0);
}

int	ft_surveil_done_eat(t_sveil *surveil)
{
	int	i;

	i = 0;
	while (i < surveil->philo_num)
	{
		sem_wait(surveil->ipc_sems->philo_done_eat);
		i++;
	}
	sem_wait(surveil->ipc_sems->print);
	sem_post(surveil->ipc_sems->finish);
	exit(0);
}
