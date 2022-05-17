/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhwang <yhwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 23:42:49 by yhwang            #+#    #+#             */
/*   Updated: 2022/05/17 16:09:40 by yhwang           ###   ########.fr       */
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

void	philo_num_is_one(t_data *data)
{
	printf("%d %d %s\n", 1, 1, "is thinking");
	printf("%d %d %s\n", 1, 1, "has taken a fork");
	printf("%d ", data->time_die + 1);
	printf("%d %s\n", 1, "has died");
}

int	setting_up_philo(t_data *data, t_philo *philo, pthread_t *thread_philo)
{
	int	i;

	i = 0;
	while (i < data->philo_num)
	{
		if (data->philo_num == 1)
		{
			philo_num_is_one(data);
			ft_free(2, data, philo, thread_philo);
			return (1);
		}
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
	{
		ft_free(1, &data, philo, NULL);
		return (1);
	}
	thread_philo = malloc(data.philo_num * sizeof(pthread_t));
	if (!thread_philo)
	{
		ft_free(2, &data, philo, thread_philo);
		return (1);
	}
	if (setting_up_philo(&data, philo, thread_philo))
		return (1);
	ft_free(2, &data, philo, thread_philo);
	return (0);
}
