/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 07:36:01 by kshim             #+#    #+#             */
/*   Updated: 2022/11/28 15:27:10 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdlib.h>

#include "./philosophers_bonus.h"

int	ft_init_surveil_argument(int argc, char **argv, t_prg *prg)
{
	memset(prg, 0, sizeof(t_prg));
	prg->surveil = (t_sveil *)malloc(sizeof(t_sveil));
	if (prg->surveil == 0)
		return (1);
	memset(prg->surveil, 0, sizeof(t_sveil));
	prg->surveil->philo_num = ft_atoi(argv[1]);
	prg->surveil->half_num = prg->surveil->philo_num / 2;
	prg->surveil->time_to_die = ft_atoi(argv[2]);
	prg->surveil->time_to_die_micro = prg->surveil->time_to_die * 1000;
	prg->surveil->time_to_eat = ft_atoi(argv[3]);
	prg->surveil->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		prg->surveil->number_to_eat = ft_atoi(argv[5]);
	else
		prg->surveil->number_to_eat = -1;
	if (prg->surveil->philo_num <= 0 || prg->surveil->time_to_die <= 0
		|| prg->surveil->time_to_eat <= 0
		|| prg->surveil->time_to_sleep <= 0
		|| (argc == 6 && prg->surveil->number_to_eat <= 0))
	{
		free(prg->surveil);
		prg->surveil = 0;
		return (1);
	}
	return (0);
}

int	ft_mem_alloc_philo_semas_pids(t_prg *prg)
{
	prg->surveil->ipc_sems = (t_ipc_sem *)malloc(sizeof(t_ipc_sem));
	if (prg->surveil->ipc_sems == 0)
		return (1);
	memset(prg->surveil->ipc_sems, 0, sizeof(t_ipc_sem));
	prg->philo = (t_philo *)malloc(sizeof(t_philo));
	if (prg->philo == 0)
		return (1);
	memset(prg->philo, 0, sizeof(t_philo));
	prg->surveil->pid_array
		= (int *)malloc(sizeof(int) * prg->surveil->philo_num);
	if (prg->surveil->pid_array == 0)
		return (1);
	memset(prg->surveil->pid_array, 0, sizeof(int) * prg->surveil->philo_num);
	prg->philo->surveil = prg->surveil;
	return (0);
}

int	ft_open_semas(t_sveil *surveil)
{
	surveil->ipc_sems->start_eat = ft_philo_sem_open("ft_philo_start_eat", 0);
	surveil->ipc_sems->done = ft_philo_sem_open("ft_philo_done", 1);
	surveil->ipc_sems->forks
		= ft_philo_sem_open("ft_philo_forks", surveil->philo_num);
	surveil->ipc_sems->napkin_odd
		= ft_philo_sem_open("ft_philo_nap_odd", 0);
	surveil->ipc_sems->napkin_even
		= ft_philo_sem_open("ft_philo_nap_even", 0);
	surveil->ipc_sems->print = ft_philo_sem_open("ft_philo_print", 1);
	surveil->ipc_sems->philo_done_eat
		= ft_philo_sem_open("ft_philo_philo_done_eat", 0);
	surveil->ipc_sems->last_eat
		= ft_philo_sem_open("ft_philo_last_eat", 1);
	if (surveil->ipc_sems->start_eat == 0 || surveil->ipc_sems->done == 0
		|| surveil->ipc_sems->forks == 0 || surveil->ipc_sems->napkin_odd == 0
		|| surveil->ipc_sems->napkin_even == 0 || surveil->ipc_sems->print == 0
		|| surveil->ipc_sems->philo_done_eat == 0
		|| surveil->ipc_sems->last_eat == 0)
		return (1);
	if (surveil->philo_num % 2 == 1)
	{
		surveil->ipc_sems->napkin_last
			= ft_philo_sem_open("ft_philo_nap_last", 0);
		if (surveil->ipc_sems->napkin_last == 0)
			return (1);
	}
	return (0);
}

// unlink를 sem_open 에러 시에 수행하고 open을 재시도하게 만드는 건 어떨까?
	// errno 사용 못해서 sem_open의 에러를 구분할 수 없는데 그렇게 만들어야 하나?
sem_t	*ft_philo_sem_open(const char *name, int value)
{
	sem_t	*tmp;

	sem_unlink(name);
	tmp = sem_open(name, O_CREAT, 0644, value);
	if (tmp == SEM_FAILED)
		return (0);
	return (tmp);
}
