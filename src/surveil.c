/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   surveil.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 10:13:09 by kshim             #+#    #+#             */
/*   Updated: 2022/11/22 15:24:27 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/philosophers.h"

int	ft_surveil_end(t_philo *philo_arr, t_sveil *surveil)
{
	int			i;

	while (1)
	{
		i = 0;
		while (i < surveil -> philo_num)
		{
			pthread_mutex_lock(philo_arr[i].last_eat);
			if (ft_set_time_after_last_eat(
					&(philo_arr[i])) >= surveil -> time_to_die)
			{
				pthread_mutex_lock(surveil -> done);
				surveil -> stop = 1;
				pthread_mutex_unlock(surveil -> done);
				pthread_mutex_lock(surveil -> print);
				if (printf("%llu %d died\n", ft_set_timestamp(&(philo_arr[i])),
						philo_arr[i].number) == -1)
					return (1);
				break ;
			}
			pthread_mutex_unlock(philo_arr[i].last_eat);
			i++;
		}
		if (surveil -> stop != 1)
		{
			pthread_mutex_lock(surveil -> done);
			if (surveil -> philo_done_eat == surveil -> philo_num)
			{
				surveil -> stop = 1;
				pthread_mutex_lock(surveil -> print);
				if (printf("done to eat\n") == -1)
					return (1);
			}
			pthread_mutex_unlock(surveil -> done);
		}
		if (surveil -> stop == 1)
			break ;
	}
	return (0);
}
