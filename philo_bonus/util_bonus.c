/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 10:31:05 by kshim             #+#    #+#             */
/*   Updated: 2022/11/28 12:46:05 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>

#include "./philosophers.h"

int	ft_atoi(const char *str)
{
	long long	ret;
	long long	tmp;
	int			sign;

	ret = 0;
	tmp = 0;
	sign = 1;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	sign = 1 - ((*str == '-') << 1);
	str += (*str == '+' || *str == '-');
	while (*str >= '0' && *str <= '9')
	{
		tmp = ret * 10 + (*str - '0');
		if ((tmp - (*(str++) - '0')) / 10 == ret)
			ret = tmp;
		else
		{
			if (sign == 1)
				return (-1);
			else if (sign == -1)
				return (0);
		}
	}
	return ((int)(ret * sign));
}

int	ft_philo_routine_only_one(t_philo *philo)
{
	while (1)
	{
		sem_wait(philo->napkin);
		sem_wait(surveil->ipc_sems->forks);
		if (ft_print_with_mutex(philo, philo->surveil, "has taken a fork") != 0)
		{
			sem_post(surveil->ipc_sems->forks);
			return (1);
		}
		ft_usleep((philo->surveil->time_to_die * 1000) + 100);
		sem_post(surveil->ipc_sems->forks);
		sem_wait(philo->surveil->ipc_sems->done);
		if (philo->surveil->stop == 1)
		{
			sem_post(philo->surveil->ipc_sems->done);
			sem_post(philo->napkin);
			break ;
		}
		sem_post(philo->napkin);
	}
	return (0);
}

int	ft_print_with_mutex(t_philo *philo, t_sveil *surveil, char *str)
{
	sem_wait(surveil->ipc_sems->print);
	sem_wait(surveil->ipc_sems->done);
	if (surveil->stop != 1)
	{
		sem_post(surveil->ipc_sems->done);
		if (printf("%llu %d %s\n",
				ft_set_timestamp(philo), philo->number, str) == -1)
		{
			sem_post(surveil->ipc_sems->print);
			return (1);
		}
		sem_post(surveil->ipc_sems->print);
	}
	else if (surveil->stop == 1)
	{
		sem_post(surveil->ipc_sems->done);
		sem_post(surveil->ipc_sems->print);
		return (1);
	}
	return (0);
}

int	ft_usleep(uint64_t sleep_time)
{
	uint64_t	target_time;
	uint64_t	now_time;

	target_time = ft_set_now_ms() + (sleep_time / 1000);
	while (1)
	{
		now_time = ft_set_now_ms();
		if (target_time <= now_time)
			return (0);
		sleep_time = (target_time - now_time) * (1000 / 2);
		if (sleep_time < 10)
			sleep_time = 10;
		usleep(sleep_time);
	}
	return (0);
}
