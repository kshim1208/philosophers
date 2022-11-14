/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 16:56:00 by kshim             #+#    #+#             */
/*   Updated: 2022/11/14 18:02:21 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


typedef struct s_program_data{
	int philo_num;
	int time_to_eat;
	int time_to_sleep;
	int number_to_eat;
	t_mntor	*monitor;
	t_philo	*philo_array;
}			t_prg;


typedef struct s_monitor{
	/* data */
}			t_mntor;

typedef struct s_philosopher{

}			t_philo;
