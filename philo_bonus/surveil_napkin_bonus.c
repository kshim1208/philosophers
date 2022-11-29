/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   surveil_napkin_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 16:41:56 by kshim             #+#    #+#             */
/*   Updated: 2022/11/29 15:08:09 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>

#include "./philosophers_bonus.h"

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
	exit(0);
}

int	ft_surveil_napkin_even(t_sveil *surveil, int type)
{
	sem_t	*nap_type;

	while (1)
	{
		if (type == E_ODD)
			nap_type = surveil->ipc_sems->napkin_odd;
		else
			nap_type = surveil->ipc_sems->napkin_even;
		ft_distribute_ret_napkin(
			surveil, type, nap_type, surveil->half_num);
		if (type == E_ODD)
			type = E_EVEN;
		else if (type == E_EVEN)
			type = E_ODD;
	}
	return (0);
}

int	ft_surveil_napkin_odd(t_sveil *surveil, int type)
{
	sem_t	*nap_type;

	while (1)
	{
		if (type == E_ODD)
			nap_type = surveil->ipc_sems->napkin_odd;
		else
			nap_type = surveil->ipc_sems->napkin_even;
		ft_distribute_ret_napkin(
			surveil, type, nap_type, surveil->half_num);
		if (type == E_LAST)
			type = E_ODD;
		else
			type++;
	}
	return (0);
}

void	ft_distribute_ret_napkin(
			t_sveil *surveil, int type, sem_t *nap_type, int num)
{
	int		i;

	if (type == E_LAST)
	{
		sem_post(surveil->ipc_sems->napkin_last);
		ft_usleep(surveil->time_to_eat * (1000 / 2));
		sem_wait(surveil->ipc_sems->napkin_last);
	}
	else
	{
		i = 0;
		while (i < num)
		{
			sem_post(nap_type);
			i++;
		}
		ft_usleep(surveil->time_to_eat * (1000 / 2));
		while (i > 0)
		{
			sem_wait(nap_type);
			i--;
		}
	}
	return ;
}
