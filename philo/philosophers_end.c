/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_end.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 16:57:16 by kshim             #+#    #+#             */
/*   Updated: 2022/11/24 09:05:02 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>

#include "./philosophers.h"

int	ft_finish_philosophers(t_prg *prg)
{
	int	i;

	i = 0;
	pthread_join(prg->surveil->surveil_end, 0);
	while (i < prg->surveil->philo_num)
	{
		pthread_join(prg->philo_arr[i].tid, 0);
		i++;
	}
	pthread_mutex_unlock(prg->surveil->print);
	ft_finish_clear_mutex(prg->fork_arr, prg->last_eat_arr,
		prg->surveil->napkin_arr, prg->surveil->philo_num);
	ft_finish_clear_surveil(prg->surveil);
	if (prg->philo_arr != 0)
		free(prg->philo_arr);
	return (0);
}

void	ft_finish_clear_mutex(pthread_mutex_t *fork_arr,
			pthread_mutex_t *last_eat_arr,
			pthread_mutex_t *napkin_arr, int philo_num)
{
	int	i;

	i = 0;
	i = 0;
	while (i < philo_num)
	{
		pthread_mutex_destroy(&(fork_arr[i]));
		pthread_mutex_destroy(&(last_eat_arr[i]));
		pthread_mutex_destroy(&(napkin_arr[i]));
		i++;
	}
	if (fork_arr != 0)
		free(fork_arr);
	if (last_eat_arr != 0)
		free(last_eat_arr);
	if (napkin_arr != 0)
		free(napkin_arr);
	return ;
}

void	ft_finish_clear_surveil(t_sveil *surveil)
{
	if (surveil != 0)
	{
		if (surveil->print != 0)
		{
			pthread_mutex_destroy(surveil->print);
			free(surveil->print);
		}
		if (surveil->done != 0)
		{
			pthread_mutex_destroy(surveil->done);
			free(surveil->done);
		}
		free(surveil);
	}
	return ;
}
