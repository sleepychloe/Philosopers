/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhwang <yhwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 03:54:24 by yhwang            #+#    #+#             */
/*   Updated: 2022/05/16 04:21:13 by yhwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/philo.h"

void	philo_print(t_philo *philo, const char *string)
{
	pthread_mutex_lock(&philo->data->printing);
	printf("%ld ", get_time_mili() - philo->data->time_start);
	printf("%d ", philo->id);
	printf("%s\n", string);
	pthread_mutex_unlock(&philo->data->printing);
}

int	monitoring(t_philo *philo, t_data *data)
{
	int	i;

	i = 0;
	while (1)
	{
		usleep(100);
		pthread_mutex_lock(&philo[i].dining);
		if (get_time_mili() - philo[i].time_last >= (size_t)data->time_die)
		{
			pthread_mutex_lock(&data->changing_state);
			if (data->state != COMPLETE)
				data->state = DEAD;
			if (data->state == DEAD)
				philo_print(&philo[i], "has died");
			pthread_mutex_unlock(&data->changing_state);
			return (1);
		}
		pthread_mutex_unlock(&philo[i].dining);
		i++;
		if (i >= data->philo_num)
			i = 0;
	}
	return (0);
}
