/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 16:56:00 by kshim             #+#    #+#             */
/*   Updated: 2022/12/02 12:02:40 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_BONUS_H
# define PHILOSOPHERS_BONUS_H

# include <pthread.h>
# include <semaphore.h>

# include <fcntl.h>

# include <stdint.h>

typedef enum e_odd_even_last{
	E_ODD = 0,
	E_EVEN = 1,
	E_LAST
}			t_eat_type;

typedef enum e_semaphore_name_array{
	E_START_EAT = 0,
	E_DONE,
	E_FORKS,
	E_NAP_ODD,
	E_NAP_EVEN,
	E_PRINT,
	E_DONE_EAT,
	E_LAST_EAT,
	E_FINISH,
	E_NAP_LAST
}			t_sem_name_arr;

typedef struct s_interprocess_semaphores{
	sem_t			*start_eat;
	sem_t			*done;
	sem_t			*last_eat;
	sem_t			*forks;
	sem_t			*napkin_odd;
	sem_t			*napkin_even;
	sem_t			*napkin_last;
	sem_t			*print;
	sem_t			*philo_done_eat;
	sem_t			*finish;
}			t_ipc_sem;

typedef struct s_surveil{
	int				philo_num;
	int				half_num;
	int				time_to_eat;
	int				time_to_sleep;
	int				stop;
	int				time_to_die;
	uint64_t		time_to_die_micro;
	int				number_to_eat;
	uint64_t		start_time;
	pid_t			*pid_array;
	pid_t			surveil_napkin;
	pid_t			surveil_done_eat;
	t_ipc_sem		*ipc_sems;
	char			**sem_name_arr;
	int				max_sem_name;
}			t_sveil;

typedef struct s_philosopher{
	int				number;
	pthread_t		surveil_end;
	uint64_t		last_eat_time;
	int				number_of_eat;
	sem_t			*napkin;
	t_sveil			*surveil;
}			t_philo;

typedef struct s_program_data{
	t_sveil			*surveil;
	t_philo			*philo;
}			t_prg;

int			ft_init_surveil_argument(int argc, char **argv, t_prg *prg);
int			ft_mem_alloc_philo_semas_pids(t_prg *prg);
int			ft_open_semas(t_sveil *surveil);
void		ft_philo_sem_open_all(t_sveil *surveil);
sem_t		*ft_philo_sem_open(const char *name, int value);

int			ft_phiosophers_start(t_prg *prg,
				t_philo *philo, t_sveil *surveil);
int			ft_philo_routine(t_philo *philo, t_sveil *surveil);
int			ft_philo_eat(t_philo *philo, t_sveil *surveil);
int			ft_philo_after_eat(t_philo *philo, t_sveil *surveil);
int			ft_philo_sleep_think(t_philo *philo, t_sveil *surveil);

int			ft_surveil_done_eat(t_sveil *surveil);
int			ft_finish_philosophers(t_prg *prg, t_sveil *surveil);
int			ft_philo_end_wait_kill(t_sveil *surveil, pid_t ret_pid);
int			ft_sem_close_unlink(t_sveil *surveil);

int			ft_atoi(const char *str);
int			ft_philo_routine_only_one(t_philo *philo);
int			ft_print_with_sema(t_philo *philo, t_sveil *surveil, char *str);
int			ft_usleep(uint64_t sleep_time);
void		ft_philo_sem_name_array(t_sveil *surveil);

uint64_t	ft_set_now_micro_s(void);
uint64_t	ft_set_timestamp(t_philo *philo);
uint64_t	ft_set_time_after_last_eat(t_philo *philo);

int			ft_surveil_end(t_philo *philo);
int			ft_surveil_end_last_eat(t_philo *philo, t_sveil *surveil);

int			ft_surveil_napkin(t_sveil *surveil);
int			ft_surveil_napkin_even(t_sveil *surveil, int type);
int			ft_surveil_napkin_odd(t_sveil *surveil, int type);
void		ft_distribute_ret_napkin(
				t_sveil *surveil, int type, sem_t *nap_type, int num);
void		ft_surveil_napkin_set(t_sveil *surveil, int set_mode);

void		ft_fork_philo(t_philo *philo, t_sveil *surveil, int i);
void		ft_philo_set_before_routine(
				t_philo *philo, t_sveil *surveil, int i);
void		ft_fork_napkin_done_eat(t_sveil *surveil);
void		ft_kill_pid_array(t_sveil *surveil);
void		ft_exit_after_unlink_sem(t_sveil *surveil);
#endif