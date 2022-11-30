/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_fork_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 14:49:57 by kshim             #+#    #+#             */
/*   Updated: 2022/11/30 17:33:13 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

#include "./philosophers_bonus.h"

void	ft_fork_philo(t_philo *philo, t_sveil *surveil, int i)
{
	surveil->start_time = ft_set_now_micro_s();
	while (i < surveil->philo_num)
	{
		surveil->pid_array[i] = fork();
		if (surveil->pid_array[i] == -1)
		{
			ft_kill_pid_array(surveil);
			ft_exit_after_unlink_sem(surveil);
		}
		else if (surveil->pid_array[i] == 0)
		{
			ft_philo_set_before_routine(philo, surveil, i);
			ft_philo_routine(philo, surveil);
		}
		i++;
	}
	return ;
}

void	ft_philo_set_before_routine(t_philo *philo, t_sveil *surveil, int i)
{
	philo->number = i + 1;
	pthread_create(&(philo->surveil_end), 0,
		(void *)ft_surveil_end, (void *)philo);
	pthread_detach(philo->surveil_end);
	if ((philo->number == surveil->philo_num)
		&& (philo->number % 2 == 1))
		philo->napkin = surveil->ipc_sems->napkin_last;
	else if (philo->number % 2 == 0)
		philo->napkin = surveil->ipc_sems->napkin_even;
	else
		philo->napkin = surveil->ipc_sems->napkin_odd;
	sem_wait(surveil->ipc_sems->start_eat);
	return ;
}

void	ft_fork_napkin_done_eat(t_sveil *surveil)
{
	surveil->surveil_napkin = fork();
	if (surveil->surveil_napkin == 0)
		ft_surveil_napkin(surveil);
	else if (surveil->surveil_napkin < 0)
	{
		ft_kill_pid_array(surveil);
		ft_exit_after_unlink_sem(surveil);
	}
	if (surveil->number_to_eat > 0)
	{
		surveil->surveil_done_eat = fork();
		if (surveil->surveil_done_eat == 0)
			ft_surveil_done_eat(surveil);
		else if (surveil->surveil_done_eat < 0)
		{
			ft_kill_pid_array(surveil);
			kill(surveil->surveil_napkin, SIGTERM);
			waitpid(-1, 0, 0);
			ft_exit_after_unlink_sem(surveil);
		}
	}
	return ;
}

void	ft_kill_pid_array(t_sveil *surveil)
{
	int	i;
	int	j;

	i = -1;
	while (i < surveil->philo_num - 1)
	{
		i++;
		if (surveil->pid_array[i] == 0)
			break ;
		else
			kill(surveil->pid_array[i], SIGTERM);
	}
	j = 0;
	while (j < i)
	{
		waitpid(-1, 0, 0);
		j++;
	}
	return ;
}

void	ft_exit_after_unlink_sem(t_sveil *surveil)
{
	ft_sem_close_unlink(surveil);
	exit(1);
}
