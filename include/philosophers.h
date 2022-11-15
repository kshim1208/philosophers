/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 16:56:00 by kshim             #+#    #+#             */
/*   Updated: 2022/11/15 12:48:37 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdint.h>
# include <sys/time.h>

typedef struct s_surveil{
	int				philo_num;
	int				time_to_eat;
	int				time_to_sleep;
	int				stop;
	int				time_to_die;
	int				number_to_eat;
	pthread_mutex_t	*print;
}			t_sveil;

typedef struct s_philosopher{
	pthread_t		tid;
	int				number;
	uint64_t		last_eat_time;
	uint64_t		start_time;
	int				number_of_eat;
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;
	struct timeval	time_value;
	t_sveil			*surveil;
}			t_philo;

typedef struct s_program_data{
	t_sveil			*surveil;
	t_philo			*philo_arr;
	pthread_mutex_t	*fork_arr;
}			t_prg;

int			ft_init_prg(int argc, char **argv, t_prg *prg);
int			ft_set_fork_mutex(pthread_mutex_t *fork_arr, int philo_num);
void		ft_set_philo(t_philo *philo_arr, int philo_num,
				pthread_mutex_t *fork_arr, t_sveil *surveil);

int			ft_phiosophers_start(t_prg *prg,
				t_philo *philo_arr, t_sveil *surveil);
void		ft_philo_routine(t_philo *philo);
void		ft_start_surveil(t_philo *philo_arr, t_sveil *surveil);
void		ft_finish_philosophers(t_prg *prg);

int			ft_atoi(const char *str);
uint64_t	ft_set_start_time(t_philo *philo);
uint64_t	ft_set_timestamp(t_philo *philo);
uint64_t	ft_set_time_after_last_eat(t_philo *philo);
void		ft_print_with_mutex(t_philo *philo, t_sveil *surveil, char *str);
#endif