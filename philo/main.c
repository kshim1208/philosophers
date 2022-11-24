/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 16:54:31 by kshim             #+#    #+#             */
/*   Updated: 2022/11/24 09:05:05 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosophers.h"

#include <stdlib.h>

int	main(int argc, char **argv)
{
	t_prg	prg;

	if (argc != 5 && argc != 6)
		return (1);
	if (ft_init_surveil_argument(argc, argv, &prg) != 0)
		return (1);
	if (ft_init_mutex_and_philo(&prg, prg.surveil) != 0)
		return (1);
	if (ft_phiosophers_start(&prg, prg.philo_arr, prg.surveil) != 0)
		return (1);
	return (0);
}
