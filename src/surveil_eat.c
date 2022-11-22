/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   surveil_eat.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 16:41:56 by kshim             #+#    #+#             */
/*   Updated: 2022/11/22 19:07:09 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/philosophers.h"

// surveil eat에서는 philo_arr 필요 없을 가능성 높음
	// surveil에서 napkin_arr에 접근 가능
int	ft_surveil_eat(t_prg *prg)
{
	t_philo	*philo_arr;
	t_sveil	*surveil;
	int		type;
	int		i;

	philo_arr = prg->philo_arr;
	surveil = prg->surveil;
	type = E_ODD;
	if (surveil->philo_num == 1)
		type = E_LAST;
	i = 0;
	while (i < surveil->philo_num)
	{
		pthread_mutex_lock(philo_arr[i].napkin);
		i++;
	}
	if (surveil->philo_num % 2 == 0)
		ft_surveil_eat_even(philo_arr, surveil, type);
	else
		ft_surveil_eat_odd(philo_arr, surveil, type);
	ft_surveil_eat_end_unlock_napkin(surveil);
	return (0);
}

int	ft_surveil_eat_even(t_philo *philo_arr, t_sveil *surveil, int type)
{
	while (1)
	{
		ft_distribue_ret_napkin(philo_arr, surveil, type, surveil->philo_num);
		if (type == E_ODD)
			type = E_EVEN;
		else if (type == E_EVEN)
			type = E_ODD;
		pthread_mutex_lock(surveil->done);
		if (surveil->stop == 1)
		{
			pthread_mutex_unlock(surveil->done);
			break ;
		}
		pthread_mutex_unlock(surveil->done);
	}
	return (0);
}

int	ft_surveil_eat_odd(t_philo *philo_arr, t_sveil *surveil, int type)
{
	while (1)
	{
		if (type == E_LAST)
		{
			pthread_mutex_unlock(philo_arr[(surveil->philo_num) - 1].napkin);
			ft_usleep(surveil->time_to_eat * (1000 / 2));
			pthread_mutex_lock(philo_arr[(surveil->philo_num) - 1].napkin);
		}
		else
			ft_distribue_ret_napkin(
				philo_arr, surveil, type, (surveil->philo_num) - 1);
		if (type == E_LAST)
			type = E_ODD;
		else
			type++;
		pthread_mutex_lock(surveil->done);
		if (surveil->stop == 1)
		{
			pthread_mutex_unlock(surveil->done);
			return (0);
		}
		pthread_mutex_unlock(surveil->done);
	}
	return (0);
}

void	ft_distribue_ret_napkin(t_philo *philo_arr,
			t_sveil *surveil, int type, int philo_num)
{
	int	i;

	i = 0;
	while (i < philo_num)
	{
		pthread_mutex_unlock(philo_arr[i + type].napkin);
		i = i + 2;
	}			
	ft_usleep(surveil->time_to_eat * (1000 / 2));
	i = i - 2;
	while (i > 0)
	{
		pthread_mutex_lock(philo_arr[i + type].napkin);
		i = i - 2;
	}
	return ;
}

void	ft_surveil_eat_end_unlock_napkin(t_sveil *surveil)
{
	int	i;

	i = 0;
	while (i < surveil->philo_num)
	{
		pthread_mutex_unlock(&(surveil->napkin_arr[i]));
		i++;
	}
	return ;
}
