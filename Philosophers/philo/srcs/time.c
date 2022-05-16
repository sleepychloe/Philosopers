/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhwang <yhwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 04:21:21 by yhwang            #+#    #+#             */
/*   Updated: 2022/05/16 04:27:59 by yhwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/philo.h"

size_t	get_time_mili(void)
{
	struct timeval	current;
	size_t			time;

	gettimeofday(&current, NULL);
	time = current.tv_sec * 1000;
	time += current.tv_usec / 1000;
	return (time);
}

size_t	get_time_micro(void)
{
	struct timeval	current;
	size_t			time;

	gettimeofday(&current, NULL);
	time = current.tv_sec;
	time *= 1000000;
	time += current.tv_usec;
	return (time);
}

void	fy_usleep(size_t timecheck)
{
	size_t	start;

	start = get_time_micro();
	timecheck *= 1000;
	while (get_time_micro() - start < timecheck)
		usleep(100);
}
