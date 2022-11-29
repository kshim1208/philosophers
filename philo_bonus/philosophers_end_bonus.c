/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_end_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 16:57:16 by kshim             #+#    #+#             */
/*   Updated: 2022/11/29 11:08:00 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include "./philosophers_bonus.h"

#include <stdio.h>

int	ft_surveil_end_philo_done_eat(t_sveil *surveil)
{
	int	i;

	i = 0;
	while (i < surveil->philo_num)
	{
		sem_wait(surveil->ipc_sems->philo_done_eat);
		i++;
	}
	sem_wait(surveil->ipc_sems->print);
	kill(surveil->pid_array[0], SIGTERM);
	return (0);
}

int	ft_finish_philosophers(t_prg *prg, t_sveil *surveil)
{
	int	i;
	int	ret_pid;

	ret_pid = waitpid(-1, 0, 0);

	sem_wait(surveil->ipc_sems->done);
	surveil->stop = 1;
	sem_post(surveil->ipc_sems->done);
	
	i = 0;
	while (i < surveil->philo_num)
	{
		if (surveil->pid_array[i] != ret_pid)
		{
			kill(surveil->pid_array[i], SIGTERM);
		}
		i++;
	}
	i = 0;
	while (i < surveil->philo_num)
	{
		if (surveil->pid_array[i] != ret_pid)
		{
			waitpid(surveil->pid_array[i], 0, 0);
		}
		i++;
	}
	printf("cccc\n");
	kill(surveil->surveil_napkin, SIGTERM);
	waitpid(surveil->surveil_napkin, 0, 0);
	if (surveil->number_to_eat > 0)
		pthread_join(surveil->surveil_done_eat, 0);
	if (surveil->philo_num % 2 == 1)
	{
		sem_close(surveil->ipc_sems->napkin_last);
		sem_unlink("ft_philo_nap_last");
	}

	sem_close(surveil->ipc_sems->start_eat);
	sem_close(surveil->ipc_sems->done);
	sem_close(surveil->ipc_sems->forks);
	sem_close(surveil->ipc_sems->napkin_odd);
	sem_close(surveil->ipc_sems->napkin_even);
	sem_close(surveil->ipc_sems->print);
	sem_close(surveil->ipc_sems->philo_done_eat);
	sem_close(surveil->ipc_sems->last_eat);

	sem_unlink("ft_philo_start_eat");
	sem_unlink("ft_philo_done");
	sem_unlink("ft_philo_forks");
	sem_unlink("ft_philo_nap_odd");
	sem_unlink("ft_philo_nap_even");
	sem_unlink("ft_philo_print");
	sem_unlink("ft_philo_philo_done_eat");
	sem_unlink("ft_philo_last_eat");

	free(surveil->ipc_sems);
	free(prg->philo);
	free(prg->surveil);
	return (0);
}
