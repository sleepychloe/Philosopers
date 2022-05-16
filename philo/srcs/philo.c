/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhwang <yhwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 01:02:50 by yhwang            #+#    #+#             */
/*   Updated: 2022/05/16 03:53:24 by yhwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/philo.h"

int	death_checker(t_philo *philo)
{
	int	sign;

	sign = 0;
	pthread_mutex_lock(&philo->data->changing_state);
	if (philo->data->state == DEAD)
		sign = 1;
	pthread_mutex_unlock(&philo->data->changing_state);
	return (sign);
}

void	philo_state(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->changing_state);
	if (philo->data->state == ALIVE)
		philo->data->state = COMPLETE;
	pthread_mutex_unlock(&philo->data->changing_state);
}

void	philo_eat(t_philo *philo, int *cnt_eat)
{
	pthread_mutex_lock(&philo->dining);
	philo->time_last = get_time_mili();
	if (*cnt_eat != -1)
		philo_print(philo, "is eating");
	*cnt_eat += 1;
	pthread_mutex_unlock(&philo->dining);
}

int	take_two_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	if (death_checker(philo))
	{
		pthread_mutex_unlock(philo->left_fork);
		return (1);
	}
	philo_print(philo, "has taken a fork");
	pthread_mutex_lock(philo->right_fork);
	if (death_checker(philo))
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return (1);
	}
	philo_print(philo, "has taken a fork");
	return (0);
}

void	*philo_start(void *param)
{
	t_philo	*philo;
	int		cnt_eat;

	philo = param;
	cnt_eat = -1;
	philo_eat(philo, &cnt_eat);
	usleep((philo->id % 2) * 200);
	while (cnt_eat != philo->data->num_must_eat && death_checker(philo) == 0)
	{
		philo_print(philo, "is thinking");
		if (take_two_forks(philo))
			break ;
		philo_eat(philo, &cnt_eat);
		fy_usleep(philo->data->time_eat);
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		if (death_checker(philo))
			break ;
		philo_print(philo, "is sleeping");
		fy_usleep(philo->data->time_sleep);
	}
	philo_state(philo);
	return (NULL);
}
