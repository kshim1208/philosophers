/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 16:54:31 by kshim             #+#    #+#             */
/*   Updated: 2022/11/17 12:45:10 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

// error 함수 만들고, 입력값으로 텍스트, 에러 코드 받아서 메인에서 반환하게 만들기?

#include <stdio.h>

int	main(int argc, char **argv)
{
	t_prg	prg;

	if (argc != 5 && argc != 6)
		return (1);
	// philosophers 구조체 시작 및 초기화
		// philosophers 메인 데이터 구조체
		// philosophers 철학자 구조체
		// philosophers 감시 구조체
	if (ft_init_prg(argc, argv, &prg) != 1)
		return (1);
	// philosophers 철학자 스레드 실행
	// 검사하다가 문제 생기면 - 철학자 사망 - 탈출하여 main까지 반환 (exit 사용 불가? 였던 것 같음)
	if (ft_phiosophers_start(&prg, prg.philo_arr, prg.surveil) != 1)
		return (1);
	return (0);
}
