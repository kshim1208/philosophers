/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_end_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 16:57:16 by kshim             #+#    #+#             */
/*   Updated: 2022/11/30 13:38:54 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include "./philosophers_bonus.h"

#include <stdio.h>

int	ft_finish_philosophers(t_prg *prg, t_sveil *surveil)
{
	sem_wait(surveil->ipc_sems->finish);
	ft_philo_end_wait_kill(surveil);
	ft_sem_close_unlink(surveil);
	free(surveil->ipc_sems);
	free(surveil->pid_array);
	free(surveil->sem_name_arr);
	free(prg->philo);
	free(prg->surveil);
	return (0);
}

int	ft_philo_end_wait_kill(t_sveil *surveil)
{
	int	i;

	if (surveil->number_to_eat != -1)
		kill(surveil->surveil_done_eat, SIGTERM);
	i = 0;
	while (i < surveil->philo_num)
	{
		kill(surveil->pid_array[i], SIGTERM);
		i++;
	}
	i = 0;
	while (i < surveil->philo_num)
	{
		waitpid(surveil->pid_array[i], 0, 0);
		i++;
	}
	kill(surveil->surveil_napkin, SIGTERM);
	waitpid(surveil->surveil_napkin, 0, 0);
	if (surveil->number_to_eat > 0)
		waitpid(surveil->surveil_done_eat, 0, 0);
	return (0);
}

int	ft_sem_close_unlink(t_sveil *surveil)
{
	if (surveil->philo_num % 2 == 1)
	{
		sem_close(surveil->ipc_sems->napkin_last);
		sem_unlink(surveil->sem_name_arr[E_NAP_LAST]);
	}
	sem_close(surveil->ipc_sems->start_eat);
	sem_close(surveil->ipc_sems->done);
	sem_close(surveil->ipc_sems->forks);
	sem_close(surveil->ipc_sems->napkin_odd);
	sem_close(surveil->ipc_sems->napkin_even);
	sem_close(surveil->ipc_sems->print);
	sem_close(surveil->ipc_sems->philo_done_eat);
	sem_close(surveil->ipc_sems->last_eat);
	sem_close(surveil->ipc_sems->finish);
	sem_unlink(surveil->sem_name_arr[E_START_EAT]);
	sem_unlink(surveil->sem_name_arr[E_DONE]);
	sem_unlink(surveil->sem_name_arr[E_FORKS]);
	sem_unlink(surveil->sem_name_arr[E_NAP_ODD]);
	sem_unlink(surveil->sem_name_arr[E_NAP_EVEN]);
	sem_unlink(surveil->sem_name_arr[E_PRINT]);
	sem_unlink(surveil->sem_name_arr[E_DONE_EAT]);
	sem_unlink(surveil->sem_name_arr[E_LAST_EAT]);
	sem_unlink(surveil->sem_name_arr[E_FINISH]);
	return (0);
}
