/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 07:36:01 by kshim             #+#    #+#             */
/*   Updated: 2022/11/30 18:51:26 by kshim            ###   ########.fr       */
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
	if (prg->surveil->philo_num <= 0 || prg->surveil->philo_num > 200
		|| prg->surveil->time_to_die <= 0 || prg->surveil->time_to_eat <= 0
		|| prg->surveil->time_to_sleep <= 0
		|| (argc == 6 && prg->surveil->number_to_eat <= 0))
		return (1);
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
		= (pid_t *)malloc(sizeof(pid_t) * prg->surveil->philo_num);
	if (prg->surveil->pid_array == 0)
		return (1);
	memset(prg->surveil->pid_array, 0, sizeof(int) * prg->surveil->philo_num);
	prg->philo->surveil = prg->surveil;
	return (0);
}

int	ft_open_semas(t_sveil *surveil)
{
	ft_philo_sem_name_array(surveil);
	ft_philo_sem_open_all(surveil);
	if (surveil->ipc_sems->start_eat == 0 || surveil->ipc_sems->done == 0
		|| surveil->ipc_sems->forks == 0 || surveil->ipc_sems->napkin_odd == 0
		|| surveil->ipc_sems->napkin_even == 0 || surveil->ipc_sems->print == 0
		|| surveil->ipc_sems->philo_done_eat == 0
		|| surveil->ipc_sems->last_eat == 0 || surveil->ipc_sems->finish == 0)
		ft_exit_after_unlink_sem(surveil);
	if (surveil->philo_num % 2 == 1)
	{
		surveil->ipc_sems->napkin_last
			= ft_philo_sem_open(surveil->sem_name_arr[E_NAP_LAST], 0);
		if (surveil->ipc_sems->napkin_last == 0)
			ft_exit_after_unlink_sem(surveil);
	}
	return (0);
}

void	ft_philo_sem_open_all(t_sveil *surveil)
{
	surveil->ipc_sems->start_eat
		= ft_philo_sem_open(surveil->sem_name_arr[E_START_EAT], 0);
	surveil->ipc_sems->done
		= ft_philo_sem_open(surveil->sem_name_arr[E_DONE], 1);
	surveil->ipc_sems->forks
		= ft_philo_sem_open(surveil->sem_name_arr[E_FORKS], surveil->philo_num);
	surveil->ipc_sems->napkin_odd
		= ft_philo_sem_open(surveil->sem_name_arr[E_NAP_ODD], 0);
	surveil->ipc_sems->napkin_even
		= ft_philo_sem_open(surveil->sem_name_arr[E_NAP_EVEN], 0);
	surveil->ipc_sems->print
		= ft_philo_sem_open(surveil->sem_name_arr[E_PRINT], 1);
	surveil->ipc_sems->philo_done_eat
		= ft_philo_sem_open(surveil->sem_name_arr[E_DONE_EAT], 0);
	surveil->ipc_sems->last_eat
		= ft_philo_sem_open(surveil->sem_name_arr[E_LAST_EAT], 1);
	surveil->ipc_sems->finish
		= ft_philo_sem_open(surveil->sem_name_arr[E_FINISH], 0);
	return ;
}

sem_t	*ft_philo_sem_open(const char *name, int value)
{
	sem_t	*tmp;

	sem_unlink(name);
	tmp = sem_open(name, O_CREAT, 0644, value);
	if (tmp == SEM_FAILED)
		return (0);
	return (tmp);
}
