/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 07:36:01 by kshim             #+#    #+#             */
/*   Updated: 2022/11/24 10:52:20 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdlib.h>

#include "./philosophers.h"

int	ft_init_surveil_argument(int argc, char **argv, t_prg *prg)
{
	memset(prg, 0, sizeof(t_prg));
	prg->surveil = (t_sveil *)malloc(sizeof(t_sveil));
	if (prg->surveil == 0)
		return (1);
	memset(prg->surveil, 0, sizeof(t_sveil));
	prg->surveil->philo_num = ft_atoi(argv[1]);
	prg->surveil->time_to_die = ft_atoi(argv[2]);
	prg->surveil->time_to_eat = ft_atoi(argv[3]);
	prg->surveil->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		prg->surveil->number_to_eat = ft_atoi(argv[5]);
	else
		prg->surveil->number_to_eat = -1;
	if (prg->surveil->philo_num <= 0 || prg->surveil->time_to_die < 0
		|| prg->surveil->time_to_eat < 0
		|| prg->surveil->time_to_sleep < 0
		|| (argc == 6 && prg->surveil->number_to_eat <= 0))
	{
		free(prg->surveil);
		prg->surveil = 0;
		return (1);
	}
	return (0);
}

int	ft_init_mutex_and_philo(t_prg *prg, t_sveil *surveil)
{
	surveil->print
		= (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	surveil->done
		= (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	surveil->napkin_arr = (pthread_mutex_t *)malloc(
			sizeof(pthread_mutex_t) * surveil->philo_num);
	prg->last_eat_arr = (pthread_mutex_t *)malloc(
			sizeof(pthread_mutex_t) * surveil->philo_num);
	prg->fork_arr = (pthread_mutex_t *)malloc(
			sizeof(pthread_mutex_t) * surveil->philo_num);
	prg->philo_arr
		= (t_philo *)malloc(sizeof(t_philo) * surveil->philo_num);
	if (surveil->print == 0 || surveil->done == 0 || prg->fork_arr == 0
		|| surveil->napkin_arr == 0 || prg->last_eat_arr == 0
		|| prg->philo_arr == 0 || pthread_mutex_init(surveil->print, 0) != 0
		|| pthread_mutex_init(surveil->done, 0) != 0
		|| ft_set_mutex_num(prg->last_eat_arr, surveil->philo_num) != 0
		|| ft_set_mutex_num(prg->fork_arr, surveil->philo_num) != 0
		|| ft_set_mutex_num(surveil->napkin_arr, surveil->philo_num) != 0)
		return (1);
	memset(prg->philo_arr, 0, sizeof(t_philo) * surveil->philo_num);
	ft_set_philo(prg->philo_arr, prg->last_eat_arr,
		prg->fork_arr, surveil);
	return (0);
}

int	ft_set_mutex_num(pthread_mutex_t *mutex_arr, int num)
{
	int	i;

	i = 0;
	while (i < num)
	{
		if (pthread_mutex_init(&(mutex_arr[i]), 0) != 0)
			return (1);
		i++;
	}
	return (0);
}

void	ft_set_philo(t_philo *philo_arr, pthread_mutex_t *last_eat_arr,
			pthread_mutex_t *fork_arr, t_sveil *surveil)
{
	int	i;

	i = 0;
	while (i < surveil->philo_num)
	{
		philo_arr[i].number = i + 1;
		philo_arr[i].first_fork = &(fork_arr[i]);
		philo_arr[i].second_fork = &(fork_arr[i + 1]);
		if (i == surveil->philo_num - 1)
			philo_arr[i].second_fork = &(fork_arr[0]);
		philo_arr[i].napkin = &(surveil->napkin_arr[i]);
		philo_arr[i].surveil = surveil;
		philo_arr[i].last_eat = &(last_eat_arr[i]);
		i++;
	}
	return ;
}
