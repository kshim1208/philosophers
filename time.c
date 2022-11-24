/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 17:58:15 by kshim             #+#    #+#             */
/*   Updated: 2022/11/24 09:05:22 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>

#include "./philosophers.h"

uint64_t	ft_set_now_ms(void)
{
	struct timeval	time_value;

	gettimeofday(&(time_value), 0);
	return (time_value.tv_sec * 1000 + time_value.tv_usec / 1000);
}

uint64_t	ft_set_timestamp(t_philo *philo)
{
	uint64_t	time;

	time = ft_set_now_ms();
	return (time - (philo->surveil->start_time));
}

uint64_t	ft_set_time_after_last_eat(t_philo *philo)
{
	uint64_t	time;

	time = ft_set_now_ms();
	return ((time
			- philo->surveil->start_time) - (philo->last_eat_time));
}
