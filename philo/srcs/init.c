/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhwang <yhwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 06:23:41 by yhwang            #+#    #+#             */
/*   Updated: 2022/05/17 06:24:27 by yhwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/philo.h"

int	init_data_fy_norminette(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_num)
	{
		if (pthread_mutex_init(&data->fork[i], NULL))
			return (1);
		i++;
	}
	if (pthread_mutex_init(&data->changing_state, NULL))
		return (1);
	if (pthread_mutex_init(&data->printing, NULL))
		return (1);
	return (0);
}

int	init_data(t_data *data, int argc, char **argv)
{
	memset(data, 0, sizeof(t_data));
	data->philo_num = ft_modified_atoi(argv[1]);
	data->time_die = ft_modified_atoi(argv[2]);
	data->time_eat = ft_modified_atoi(argv[3]);
	data->time_sleep = ft_modified_atoi(argv[4]);
	data->num_must_eat = -1;
	data->state = ALIVE;
	if (argc == 6)
		data->num_must_eat = ft_modified_atoi(argv[5]);
	if (data->philo_num <= 0 || data->time_die <= 0 || data->time_eat <= 0
		|| data->time_sleep <= 0 || (argc == 6 && data->num_must_eat < 0))
		return (1);
	data->fork = malloc(data->philo_num * sizeof(pthread_mutex_t));
	if (!data->fork)
	{
		free(data->fork);
		return (1);
	}
	if (init_data_fy_norminette(data))
	{
		free(data->fork);
		return (1);
	}
	data->time_start = get_time_mili();
	return (0);
}
