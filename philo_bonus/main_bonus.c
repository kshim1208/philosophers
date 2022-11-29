/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 16:54:31 by kshim             #+#    #+#             */
/*   Updated: 2022/11/29 16:07:57 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosophers_bonus.h"

int	main(int argc, char **argv)
{
	t_prg	prg;

	if (argc != 5 && argc != 6)
		return (1);
	if (ft_init_surveil_argument(argc, argv, &prg) != 0)
		return (1);
	if (ft_mem_alloc_philo_semas_pids(&prg) != 0)
		return (1);
	if (ft_open_semas(prg.surveil) != 0)
		return (1);
	if (ft_phiosophers_start(&prg, prg.philo, prg.surveil) != 0)
		return (1);
	return (0);
}
