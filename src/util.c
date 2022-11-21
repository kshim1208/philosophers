/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 10:31:05 by kshim             #+#    #+#             */
/*   Updated: 2022/11/21 14:55:08 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>

#include "../include/philosophers.h"

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

// 밑의 두 함수 제대로 작성한거 맞나?
	// 제대로 작성하였다면, 이 두 함수를 합칠 수 있을까? 그럴 이유와 장단점은 있는가?
uint64_t	ft_set_now_ms(void)
{
	struct timeval	time_value;

	if (gettimeofday(&(time_value), 0) != 0)
		return (-1);
	return (time_value.tv_sec * 1000 + time_value.tv_usec / 1000);
}

uint64_t	ft_set_timestamp(t_philo *philo)
{
	uint64_t	time;

	time = ft_set_now_ms();
	if (time == -1)
		return (-1);
	return (time - (philo -> surveil -> start_time));
}

uint64_t	ft_set_time_after_last_eat(t_philo *philo)
{
	uint64_t	time;

	time = ft_set_now_ms();
	if (time == -1)
		return (-1);
	return ((time
			- philo -> surveil -> start_time) - (philo -> last_eat_time));
}

// error 예외처리 필요
int	ft_print_with_mutex(t_philo *philo, t_sveil *surveil, char *str)
{
	if (surveil -> stop != 1)
	{
		pthread_mutex_lock(surveil -> print);
		printf("%llu %d %s\n", ft_set_timestamp(philo), philo -> number, str);
		pthread_mutex_unlock(surveil -> print);
		return (0);
	}
	return (1);
}

int	ft_usleep(uint64_t sleep_time)
{
	uint64_t	target_time;
	uint64_t	now_time;
	int			ret;

	target_time = ft_set_now_ms() + (sleep_time / 1000);
	while (1)
	{
		now_time = ft_set_now_ms();
		if (target_time <= now_time)
			return (0);
		sleep_time = (target_time - now_time) * (1000 / 2);
		ret = usleep(sleep_time);
		if (ret != 0)
			return (ret);
	}
	return (0);
}
