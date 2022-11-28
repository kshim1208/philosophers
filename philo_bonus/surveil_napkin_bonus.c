/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   surveil_napkin_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 16:41:56 by kshim             #+#    #+#             */
/*   Updated: 2022/11/29 08:47:22 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>

#include "./philosophers_bonus.h"

#include <stdio.h>

int	ft_surveil_napkin(t_sveil *surveil)
{
	int		type;

	type = E_ODD;
	if (surveil->philo_num == 1)
		type = E_LAST;
	if (surveil->philo_num % 2 == 0)
		ft_surveil_napkin_even(surveil, type);
	else
		ft_surveil_napkin_odd(surveil, type);
	return (0);
}

int	ft_surveil_napkin_even(t_sveil *surveil, int type)
{
	while (1)
	{
		sem_wait(surveil->ipc_sems->done);
		if (surveil->stop != 1)
		{
			sem_post(surveil->ipc_sems->done);
			ft_distribue_ret_napkin(
				surveil, type, surveil->half_num);
			if (type == E_ODD)
				type = E_EVEN;
			else if (type == E_EVEN)
				type = E_ODD;
		}
		else
		{
			sem_post(surveil->ipc_sems->done);
			break ;
		}
	}
	return (0);
}

int	ft_surveil_napkin_odd(t_sveil *surveil, int type)
{
	while (1)
	{
		sem_wait(surveil->ipc_sems->done);
		if (surveil->stop != 1)
		{
			sem_post(surveil->ipc_sems->done);
			ft_distribue_ret_napkin(
				surveil, type, surveil->half_num);
			if (type == E_LAST)
				type = E_ODD;
			else
				type++;
		}
		else
		{
			sem_post(surveil->ipc_sems->done);
			break ;
		}
	}
	return (0);
}

void	ft_distribue_ret_napkin(
			t_sveil *surveil, int type, int num)
{
	int		i;
	sem_t	*napkin_addr;

	if (type == E_LAST)
	{
		sem_post(surveil->ipc_sems->napkin_last);
		ft_usleep(surveil->time_to_eat * (1000 / 2));
		sem_wait(surveil->ipc_sems->napkin_last);
	}
	else
	{
		if (type == E_ODD)
			napkin_addr = surveil->ipc_sems->napkin_odd;
		else
			napkin_addr = surveil->ipc_sems->napkin_even;
		i = 0;
		while (i < num)
		{
			sem_post(napkin_addr);
			i++;
		}
		ft_usleep(surveil->time_to_eat * (1000 / 2));
		i--;
		while (i > 0)
		{
			sem_wait(napkin_addr);
			i--;
		}
	}
	return ;
}
