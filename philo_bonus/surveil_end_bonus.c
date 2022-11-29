/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   surveil_end_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 16:41:47 by kshim             #+#    #+#             */
/*   Updated: 2022/11/29 11:17:15 by kshim            ###   ########.fr       */
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
		sem_wait(surveil->ipc_sems->done);
		if (surveil->stop != 1)
		{
			sem_post(surveil->ipc_sems->done);
			if (ft_surveil_end_last_eat(philo, surveil) != 0)
				exit(1);
		}
		else
		{
			sem_post(surveil->ipc_sems->done);
			exit(0);
		}
	}
	return (0);
}

int	ft_surveil_end_last_eat(t_philo *philo, t_sveil *surveil)
{
	sem_wait(surveil->ipc_sems->last_eat);
	if (ft_set_time_after_last_eat(
			philo) >= (uint64_t)surveil->time_to_die_micro)
	{
		sem_wait(surveil->ipc_sems->print);
		sem_post(surveil->ipc_sems->last_eat);
		sem_wait(surveil->ipc_sems->done);
		surveil->stop = 1;
		sem_post(surveil->ipc_sems->done);
		if (printf("%llu %d died\n", ft_set_timestamp(philo) / 1000,
				philo->number) == -1)
		{
			exit(1);
		}
		exit(0);
	}
	sem_post(surveil->ipc_sems->last_eat);
	return (0);
}
