/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 16:56:00 by kshim             #+#    #+#             */
/*   Updated: 2022/11/28 15:20:42 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_BONUS_H
# define PHILOSOPHERS_BONUS_H

# include <pthread.h>

# include <fcntl.h>           /* For O_* constants */

# include <semaphore.h>
# include <stdint.h>

typedef enum e_odd_even_last{
	E_ODD = 0,
	E_EVEN = 1,
	E_LAST
}			t_eat_type;

typedef enum e_lock_unlock_napkin{
	E_LOCK = 0,
	E_UNLOCK
}			t_l_u_lock_napkin;

typedef enum e_fork_state{
	E_DROP = 0,
	E_HOLD
}			t_fork_state;

typedef struct s_interprocess_semaphores{
	sem_t			*start_eat;
	sem_t			*done;
	sem_t			*forks;
	sem_t			*napkin_odd;
	sem_t			*napkin_even;
	sem_t			*napkin_last;
	sem_t			*print;
	sem_t			*philo_done_eat;
	sem_t			*last_eat;
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
	pthread_t		surveil_napkin;
	pthread_t		surveil_done_eat;
	t_ipc_sem		*ipc_sems;
	int				*pid_array;
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
sem_t		*ft_philo_sem_open(const char *name, int value);

int			ft_phiosophers_start(t_prg *prg,
				t_philo *philo, t_sveil *surveil);
int			ft_philo_routine(t_philo *philo, t_sveil *surveil);
int			ft_philo_eat(t_philo *philo, t_sveil *surveil);
int			ft_philo_after_eat(t_philo *philo, t_sveil *surveil);
int			ft_philo_sleep_think(t_philo *philo, t_sveil *surveil);

int			ft_surveil_end_philo_done_eat(t_sveil *surveil);
int			ft_finish_philosophers(t_prg *prg, t_sveil *surveil);

int			ft_atoi(const char *str);
int			ft_philo_routine_only_one(t_philo *philo);
int			ft_print_with_mutex(t_philo *philo, t_sveil *surveil, char *str);
int			ft_usleep(uint64_t sleep_time);

uint64_t	ft_set_now_micro_s(void);
uint64_t	ft_set_timestamp(t_philo *philo);
uint64_t	ft_set_time_after_last_eat(t_philo *philo);

int			ft_surveil_end(t_philo *philo);
int			ft_surveil_end_last_eat(t_philo *philo, t_sveil *surveil);

int			ft_surveil_napkin(t_sveil *surveil);
int			ft_surveil_napkin_even(t_sveil *surveil, int type);
int			ft_surveil_napkin_odd(t_sveil *surveil, int type);
void		ft_distribue_ret_napkin(
				t_sveil *surveil, int type, int num);
void		ft_surveil_napkin_set(t_sveil *surveil, int set_mode);

#endif