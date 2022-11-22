/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_end.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 16:57:16 by kshim             #+#    #+#             */
/*   Updated: 2022/11/22 17:44:38 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>

#include "../include/philosophers.h"

void	ft_philo_mutex_unlock(t_philo *philo, t_sveil *surveil)
{
	if (philo -> mutex_lock_check[E_FIRST_FORK] == 1)
		pthread_mutex_unlock(philo -> first_fork);
	if (philo -> mutex_lock_check[E_SECOND_FORK] == 1)
		pthread_mutex_unlock(philo -> second_fork);
	if (philo -> mutex_lock_check[E_PRINT] == 1)
		pthread_mutex_unlock(surveil -> print);
	if (philo -> mutex_lock_check[E_DONE] == 1)
		pthread_mutex_unlock(surveil -> done);
	return ;
}

// pthread 해제, mutex 해제, prg 디렉토리 해제 함수를 분리 
	// intilize 단계에서 실패했을 때도 일부 사용할 수 있도록

	// 1개만 있을 때 join or detach 필요함
int	ft_finish_philosophers(t_prg *prg)
{
	int	i;

	i = 0;
	pthread_join(prg -> surveil -> surveil_eat, 0);
	while (i < prg -> surveil -> philo_num)
	{
		pthread_join(prg -> philo_arr[i].tid, 0);
		i++;
	}
	pthread_mutex_unlock(prg -> surveil -> print);
	i = 0;
	while (i < prg -> surveil -> philo_num)
	{
		pthread_mutex_destroy(&(prg -> fork_arr[i]));
		pthread_mutex_destroy(&(prg -> last_eat_arr[i]));
		pthread_mutex_destroy(&(prg -> surveil -> napkin_arr[i]));
		i++;
	}
	if (prg -> fork_arr != 0)
		free(prg -> fork_arr);
	if (prg -> last_eat_arr != 0)
		free(prg -> last_eat_arr);
	if (prg -> surveil -> napkin_arr != 0)
		free(prg -> surveil -> napkin_arr);
	if (prg -> surveil != 0)
	{
		if (prg -> surveil -> print != 0)
		{
			pthread_mutex_destroy(prg -> surveil -> print);
			free(prg -> surveil -> print);
		}
		if (prg -> surveil -> done != 0)
		{
			pthread_mutex_destroy(prg -> surveil -> done);
			free(prg -> surveil -> done);
		}
		free(prg -> surveil);
	}
	if (prg -> philo_arr != 0)
		free(prg -> philo_arr);
	return (0);
}
