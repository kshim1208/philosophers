/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 17:58:15 by kshim             #+#    #+#             */
/*   Updated: 2022/11/28 15:09:05 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>

#include "./philosophers_bonus.h"

uint64_t	ft_set_now_micro_s(void)
{
	struct timeval	time_value;

	gettimeofday(&(time_value), 0);
	return (time_value.tv_sec * 1000000 + time_value.tv_usec);
}

uint64_t	ft_set_timestamp(t_philo *philo)
{
	uint64_t	time;

	time = ft_set_now_micro_s();
	return ((time - (philo->surveil->start_time)));
}

uint64_t	ft_set_time_after_last_eat(t_philo *philo)
{
	uint64_t	time;

	time = ft_set_now_micro_s();
	return (((time
				- philo->surveil->start_time) - (philo->last_eat_time)));
}
