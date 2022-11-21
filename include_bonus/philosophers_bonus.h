/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 22:52:52 by kshim             #+#    #+#             */
/*   Updated: 2022/11/21 22:53:33 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_BONUS_H
# define PHILOSOPHERS_BONUS_H

# include <pthread.h>
# include <stdint.h>
# include <sys/time.h>

// mutex 대신 sem 사용 예정
// pthread_t 대신 프로세스 id 사용할 예정

typedef enum e_odd_even_last{
	E_ODD = 0,
	E_EVEN,
	E_LAST
}			t_eat_type;

typedef struct s_surveil{
	int				philo_num;
	int				time_to_eat;
	int				time_to_sleep;
	int				stop;
	int				time_to_die;
	int				number_to_eat;
	int				philo_done_eat;
	pthread_mutex_t	*print;
	pthread_mutex_t	*done_eat;
	uint64_t		start_time;
	pthread_t		surveil_eat;
}			t_sveil;

typedef struct s_philosopher{
	pthread_t		tid;
	int				number;
	uint64_t		last_eat_time;
	int				time_checked;
	int				number_of_eat;
	pthread_mutex_t	*last_eat;
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;
	t_sveil			*surveil;
}			t_philo;

typedef struct s_program_data{
	t_sveil			*surveil;
	t_philo			*philo_arr;
	pthread_mutex_t	*fork_arr;
	pthread_mutex_t	*last_eat_arr;
}			t_prg;


#endif