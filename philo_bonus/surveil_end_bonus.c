/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   surveil_end_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 16:41:47 by kshim             #+#    #+#             */
/*   Updated: 2022/11/28 14:33:05 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "./philosophers.h"

int	ft_surveil_end(t_philo *philo)
{
	int			i;
	t_sveil		*surveil;

	surveil = philo->surveil;
	while (1)
	{
		sem_wait(surveil->ipc_sems->done);
		if (surveil->stop != 1)
		{
			sem_post(surveil->ipc_sems->done);
			if (ft_surveil_end_last_eat(philo, surveil) != 0)
				return (1);
		}
		else
		{
			sem_post(surveil->ipc_sems->done);
			break ;
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
		if (printf("%llu %d died\n", ft_set_timestamp(philo),
				philo->number) == -1)
		{
			sem_post(surveil->ipc_sems->print);
			return (1);
		}
		sem_post(surveil->ipc_sems->print);
		return (0);
	}
	sem_post(surveil->ipc_sems->last_eat);
	return (0);
}

int	ft_surveil_end_philo_done_eat(t_sveil *surveil)
{
	int	i;

	i = 0;
	while (i < surveil->philo_num)
	{
		sem_wait(surveil->ipc_sems->philo_done_eat);
		i++;
	}
	// 다 먹었다는 뜻 -> 출력 정지 필요
}
