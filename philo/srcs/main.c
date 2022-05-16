/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhwang <yhwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 23:42:49 by yhwang            #+#    #+#             */
/*   Updated: 2022/05/16 00:45:28 by yhwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/philo.h"

int	init_philo(t_philo *philo, t_data *data, int i)
{
	memset(philo, 0, sizeof(t_philo));
	philo->id = i + 1;
	philo->data = data;
	philo->left_fork = &data->fork[i];
	philo->right_fork = &data->fork[(i + 1) % data->philo_num];
	philo->time_last = get_time_mili();
	if (pthread_mutex_init(&philo->dining, NULL))
		return (1);
	return (0);
}

int	setting_up_philo(t_data *data, t_philo *philo, pthread_t *thread_philo)
{
	int	i;

	i = 0;
	while (i < data->philo_num)
	{
		if (init_philo(&philo[i], data, i))
			return (1);
		if (pthread_create(&thread_philo[i], NULL, philo_start, &philo[i]))
			return (1);
		i++;
	}
	monitoring(philo, data);
	i = 0;
	while (i < data->philo_num)
	{
		pthread_join(thread_philo[i], NULL);
		i++;
	}
	return (0);
}

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
	if (data->philo_num <= 0 || data->time_die <= 0|| data->time_eat <= 0
		|| data->time_sleep <= 0 || (argc == 6 && data->num_must_eat <0))
		return (1);
	data->fork = malloc(data->philo_num * sizeof(pthread_mutex_t));
	if (!data->fork)
		return (1);
	if (init_data_fy_norminette(data))
	{
		free(data->fork);
		return (1);
	}
	data->time_start = get_time_mili();
	return (0);
}

int	main(int argc, char **argv)
{
	t_data		data;
	t_philo		*philo;
	pthread_t	*thread_philo;

	if (argc < 5 || argc > 6 || init_data(&data, argc, argv))
	{
		err_msg();
		return (1);
	}
	philo = malloc(data.philo_num * sizeof(t_philo));
	if (!philo)
		return (1);
	thread_philo = malloc(data.philo_num * sizeof(pthread_t));
	if (!thread_philo)
		return (1);
	if (setting_up_philo(&data, philo, thread_philo))
		return (1);
	free(data.fork);
	free(philo);
	free(thread_philo);
	return (0);
}
