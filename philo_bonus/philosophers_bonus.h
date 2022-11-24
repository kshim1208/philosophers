/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 16:56:00 by kshim             #+#    #+#             */
/*   Updated: 2022/11/24 10:52:29 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_BONUS_H
# define PHILOSOPHERS_BONUS_H

# include <pthread.h>
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

typedef struct s_surveil{
	int				philo_num;
	int				time_to_eat;
	int				time_to_sleep;
	int				stop;
	int				time_to_die;
	int				number_to_eat;
	int				philo_done_eat;
	pthread_mutex_t	*print;
	pthread_mutex_t	*done;
	uint64_t		start_time;
	pthread_t		surveil_end;
	pthread_mutex_t	*napkin_arr;
}			t_sveil;

typedef struct s_philosopher{
	pthread_t		tid;
	int				number;
	uint64_t		last_eat_time;
	int				number_of_eat;
	pthread_mutex_t	*last_eat;
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;
	pthread_mutex_t	*napkin;
	t_sveil			*surveil;
}			t_philo;

typedef struct s_program_data{
	t_sveil			*surveil;
	t_philo			*philo_arr;
	pthread_mutex_t	*fork_arr;
	pthread_mutex_t	*last_eat_arr;
}			t_prg;

int			ft_init_surveil_argument(int argc, char **argv, t_prg *prg);
int			ft_init_mutex_and_philo(t_prg *prg, t_sveil *surveil);
int			ft_set_mutex_num(pthread_mutex_t *mutex_arr, int num);
void		ft_set_philo_only_one(t_philo *philo_arr,
				pthread_mutex_t *last_eat_arr,
				pthread_mutex_t *fork_arr, t_sveil *surveil);
void		ft_set_philo(t_philo *philo_arr, pthread_mutex_t *last_eat_arr,
				pthread_mutex_t *fork_arr, t_sveil *surveil);

int			ft_phiosophers_start(t_prg *prg,
				t_philo *philo_arr, t_sveil *surveil);
int			ft_philo_routine_only_one(t_philo *philo);
int			ft_philo_routine(t_philo *philo);
int			ft_philo_eat(t_philo *philo, t_sveil *surveil);
int			ft_philo_end_or_wait(t_philo *philo, t_sveil *surveil);

int			ft_finish_philosophers(t_prg *prg);
void		ft_finish_clear_mutex(pthread_mutex_t *fork_arr,
				pthread_mutex_t *last_eat_arr,
				pthread_mutex_t *napkin_arr, int philo_num);
void		ft_finish_clear_surveil(t_sveil *surveil);

int			ft_atoi(const char *str);
int			ft_print_with_mutex(t_philo *philo, t_sveil *surveil, char *str);
int			ft_usleep(uint64_t sleep_time);

uint64_t	ft_set_now_ms(void);
uint64_t	ft_set_timestamp(t_philo *philo);
uint64_t	ft_set_time_after_last_eat(t_philo *philo);

int			ft_surveil_end(t_prg *prg);
int			ft_surveil_end_last_eat(t_philo *philo_arr, t_sveil *surveil);

int			ft_surveil_eat(t_philo *philo_arr, t_sveil *surveil);
int			ft_surveil_eat_even(t_philo *philo_arr, t_sveil *surveil, int type);
int			ft_surveil_eat_odd(t_philo *philo_arr, t_sveil *surveil, int type);
void		ft_distribue_ret_napkin(t_philo *philo_arr,
				t_sveil *surveil, int type, int philo_num);
void		ft_surveil_eat_set_napkin(t_sveil *surveil, int set_mode);

#endif