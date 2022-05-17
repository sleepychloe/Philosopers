/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhwang <yhwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 00:36:24 by yhwang            #+#    #+#             */
/*   Updated: 2022/05/16 01:02:19 by yhwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/philo.h"

int	ft_modified_atoi(const char *str)
{
	int			sign;
	long long	ret;
	long long	nbr;

	sign = 1;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -sign;
		str++;
	}
	ret = 0;
	while (*str)
	{
		if ('0' <= *str && *str <= '9')
			ret = ret * 10 + (*str - '0');
		else
			return (-1);
		str++;
	}
	nbr = sign * ret;
	if (nbr < -2147483648 || nbr > 2147483647)
		return (-1);
	return (nbr);
}

void	ft_free(int flag, t_data *data, t_philo *philo, pthread_t *thread_philo)
{
	if (flag == 1)
	{
		free(data->fork);
		free(philo);
	}
	else if (flag == 2)
	{
		free(data->fork);
		free(philo);
		free(thread_philo);
	}
}

void	err_msg(void)
{
	printf("************************************************************\n");
	printf("********************* Invalid argument *********************\n");
	printf("************************************************************\n");
	printf("------------------------------------------------------------\n");
	printf("--command : ./philo number_of_philosophers------------------\n");
	printf("------------------------------------------------------------\n");
	printf("---------------time_to_die time_to_eat time_to_sleep--------\n");
	printf("------------------------------------------------------------\n");
	printf("---------------[number_of_times_each_philosopher_must_eat]--\n");
	printf("------------------------------------------------------------\n");
	printf("************************************************************\n");
	printf("***** The number of philosophers must be at least one! *****\n");
	printf("************************************************************\n");
}
