/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 07:36:01 by kshim             #+#    #+#             */
/*   Updated: 2022/11/15 12:38:15 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdlib.h>

#include "../include/philosophers.h"

int	ft_init_prg(int argc, char **argv, t_prg *prg)
{
	memset(prg, 0, sizeof(t_prg));
	// t_sveil 내부 설정
	prg -> surveil = (t_sveil *)malloc(sizeof(t_sveil));
	memset(prg -> surveil, 0, sizeof(t_sveil));
		// 함수 구성하고 내부 변수 설정하자
	// prg 내부 변수 설정
	prg -> surveil -> philo_num = ft_atoi(argv[1]);
	prg -> surveil -> time_to_die = ft_atoi(argv[2]);
	prg -> surveil -> time_to_eat = ft_atoi(argv[3]);
	prg -> surveil -> time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		prg -> surveil -> number_to_eat = ft_atoi(argv[5]);
	else
		prg -> surveil -> number_to_eat = -1;
	// mutex_init
	prg -> surveil -> print = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	if (prg -> surveil -> print == 0)
		return (0);
	if (pthread_mutex_init(prg -> surveil -> print, 0) != 0)
		return (0);
	prg -> fork_arr = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * prg -> surveil -> philo_num);
	if (prg -> fork_arr == 0)
		return (0);
	if (ft_set_fork_mutex(prg -> fork_arr, prg -> surveil -> philo_num) == 0)
		return (0);
	// t_philo 내부 설정
	prg -> philo_arr = (t_philo *)malloc(sizeof(t_philo) * prg -> surveil -> philo_num);
	if (prg -> philo_arr == 0)
		return (0);
		// 함수 구성하고 내부 필요 변수 설정
	ft_set_philo(prg -> philo_arr, prg -> surveil -> philo_num, prg -> fork_arr, prg -> surveil);
			// 반복문으로 number, left_fork, right_fork 설정
			// tid는 pthread_create가, time은 스레드 분기 직전에 설정
	return (1);
}

int	ft_set_fork_mutex(pthread_mutex_t *fork_arr, int philo_num)
{
	int	i;

	i = 0;
	while (i < philo_num)
	{
		pthread_mutex_init(&(fork_arr[i]), 0);
		i++;
	}
	return (1);
}

	// philo 1명일 때 처리 나중에 넣기
void	ft_set_philo(t_philo *philo_arr, int philo_num,
			pthread_mutex_t *fork_arr, t_sveil *surveil)
{
	int	i;

	memset(philo_arr, 0, sizeof(t_philo) * philo_num);
	i = 0;
	while (i < philo_num - 1)
	{
		philo_arr[i].number = i + 1;
		if (philo_arr[i].number % 2 == 1)
		{
			philo_arr[i].first_fork = &(fork_arr[i]);
			philo_arr[i].second_fork = &(fork_arr[i + 1]);
		}
		else
		{
			philo_arr[i].first_fork = &(fork_arr[i + 1]);
			philo_arr[i].second_fork = &(fork_arr[i]);
		}
		philo_arr[i].surveil = surveil;
		i++;
	}
	i++;
	philo_arr[i].number = i + 1;
	if (philo_arr[i].number % 2 == 1)
	{
		philo_arr[i].first_fork = &(fork_arr[i]);
		philo_arr[i].second_fork = &(fork_arr[0]);
	}
	else
	{
		philo_arr[i].first_fork = &(fork_arr[0]);
		philo_arr[i].second_fork = &(fork_arr[i]);
	}
	philo_arr[i].surveil = surveil;
	return ;
}
