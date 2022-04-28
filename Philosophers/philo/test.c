/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhwang <yhwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 22:26:57 by yhwang            #+#    #+#             */
/*   Updated: 2022/04/28 04:49:47 by yhwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

typedef enum e_cond
{
	TAKEN,
	EATING,
	SLEEPING,
	THINKING,
	DIED,
	COMPLETE,
}	t_status;

typedef struct s_data
{
	int					philo_num;
	int					time_die;
	int					time_eat;
	int					time_sleep;
	int					num_must_eat;
	int					cnt_full;
	pthread_mutex_t		*fork;
	pthread_mutex_t		printing;
	pthread_mutex_t		dining;
	size_t				time_start;
}		t_data;

typedef struct s_philo
{
	int				i;
	int				l;
	int				r;
	int				cnt_eat;
	pthread_t		routine;
	pthread_t		monitor;
	t_data			*data;
	size_t			cur;
}		t_philo;

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

void	*ft_calloc(size_t n, size_t size)
{
	void	*mem;

	mem = (void *)malloc(n * size);
	if (mem == 0)
		return (0);
	memset(mem, 0, n * size);
	return (mem);
}

int	init_data1(int argc, char **argv, t_data *data)
{
	data->philo_num = ft_modified_atoi(argv[1]);
	if (!data->philo_num)
		return (0);
	data->time_die = ft_modified_atoi(argv[2]);
	if (!data->time_die)
		return (0);
	data->time_eat = ft_modified_atoi(argv[3]);
	if (!data->time_eat)
		return (0);
	data->time_sleep = ft_modified_atoi(argv[4]);
	if (!data->time_sleep)
		return (0);
	if (argc == 6)
	{
		data->num_must_eat = ft_modified_atoi(argv[5]);
		if (!data->num_must_eat)
			return (0);
	}
	data->fork = ft_calloc(data->philo_num, sizeof(pthread_mutex_t));
	if (data->fork == NULL)
		return (0);
	return (1);
}

int	init_data2(int argc, char **argv, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_num)
	{
		if (pthread_mutex_init(&data->fork[i], NULL))
			return (0);
		i++;
	}
	if (pthread_mutex_init(&data->dining, NULL))
		return (0);
	if (pthread_mutex_init(&data->printing, NULL))
		return (0);
	return (1);
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

int	destroy_mutex(t_data *data)
{
	int	i;

	i = 0;
	if (data->fork)
	{
		while (i < data->philo_num)
		{
			pthread_mutex_destroy(&data->fork[i]);
			i++;
		}
	}
	pthread_mutex_destroy(&data->dining);
	pthread_mutex_destroy(&data->printing);
	return (1);
}

size_t	timestamp(size_t *time_start)
{
	struct timeval	current;

	gettimeofday(&current, 0);
	if (gettimeofday(&current, 0) == -1)
		return (0);
	*time_start = (size_t)(current.tv_sec * 1000 + current.tv_usec / 1000);
	return (1);
}

void	wait_interval(t_philo *philo, size_t start, size_t interval)
{
	size_t	cur;

	while (1)
	{
		if (!timestamp(&cur))
			pthread_mutex_unlock(&philo->data->dining);
		if (cur - start >= interval)
			break ;
		if (usleep(10) == -1)
			pthread_mutex_unlock(&philo->data->dining);
	}
}

void	take_fork(t_philo *philo)
{
	if (pthread_mutex_lock(&philo->data->fork[philo->l])
		|| !print_status(TAKEN, philo)
		|| pthread_mutex_lock(&philo->data->fork[philo->r]))
		pthread_mutex_unlock(&philo->data->dining);
}

void	put_fork(t_philo *philo)
{
	if (pthread_mutex_unlock(&philo->data->fork[philo->l])
		|| pthread_mutex_unlock(&philo->data->fork[philo->r]))
		pthread_mutex_unlock(&philo->data->dining);
}

void	philo_eat(t_philo *philo)
{
	if (!timestamp(&(philo->cur)) || !print_status(EATING, philo))
		pthread_mutex_unlock(&philo->data->dining);
	wait_interval(philo, philo->cur, philo->data->time_eat);
	(philo->cnt_eat)++;
	if (philo->cnt_eat == philo->data->num_must_eat)
	{
		(philo->data->cnt_full)++;
		if (philo->data->cnt_full >= philo->data->philo_num)
			if (!print_status(COMPLETE, philo))
				pthread_mutex_unlock(&philo->data->dining);
	}
}

void	philo_sleep(t_philo *philo)
{
	size_t	cur;

	cur = 0;
	if (!timestamp(&cur) || !print_status(SLEEPING, philo))
		pthread_mutex_unlock(&philo->data->dining);
	wait_interval(philo, cur, philo->data->time_sleep);
}

void	philo_think(t_philo *philo)
{
	if (!print_status(THINKING, philo))
		pthread_mutex_unlock(&philo->data->dining);
}

void	*routine(void *ptr_ith_philo)
{
	t_philo	*philo;

	philo = (t_philo *)ptr_ith_philo;
	if (!timestamp(&philo->cur))
		pthread_mutex_unlock(&philo->data->dining);
	if (philo->i % 2)
		wait_interval(philo, philo->cur, 10 * 10);
	while (1)
	{
		take_fork(philo);
		philo_eat(philo);
		put_fork(philo);
		philo_sleep(philo);
		philo_think(philo);
	}
	return (NULL);
}

void	print_msg(t_status status)
{
	if (status == TAKEN)
		printf(" has taken a fork\n");
	else if (status == EATING)
		printf(" is eating\n");
	else if (status == SLEEPING)
		printf(" is sleeping\n");
	else if (status == THINKING)
		printf(" is thinking\n");
	else if (status == DIED)
		printf(" died\n");
}

int	print_status(t_status status, t_philo *philo)
{
	size_t	cur;

	if (pthread_mutex_lock(&philo->data->printing) || !timestamp(&cur))
		return (0);
	printf("%ld ", cur - philo->data->time_start);
	if (status == COMPLETE)
		printf("complete\n");
	else
	{
		printf("%d", philo->i + 1);
		print_msg(status);
	}
	if (status == DIED || status == COMPLETE)
		return (0);
	if (pthread_mutex_unlock(&philo->data->printing))
		return (0);
	return (1);
}

void	*monitor(void *ptr_ith_philo)
{
	t_philo		*philo;
	size_t		cur;

	philo = (t_philo *)ptr_ith_philo;
	if (!timestamp(&cur))
		pthread_mutex_unlock(&philo->data->dining);
	wait_interval(philo, cur, philo->data->time_die - 10);
	while (1)
	{
		if (!timestamp(&cur))
			pthread_mutex_unlock(&philo->data->dining);
		if (cur - philo->cur >= (size_t)philo->data->time_die)
		{
			print_status(DIED, philo);
			pthread_mutex_unlock(&philo->data->dining);
		}
	}
	return (NULL);
}

int	feed_philo(t_data *data, t_philo *philo)
{
	int	i;

	if (!timestamp(&data->time_start))
		return (0);
	if (pthread_mutex_lock(&data->dining))
		return (0);
	i = 0;
	while (i < data->philo_num)
	{
		philo[i].i = i;
		philo[i].data = data;
		philo[i].l = i;
		philo[i].r = (i + 1) % data->philo_num;
		if (pthread_create(&philo[i].routine, NULL, routine, &philo[i])
			|| pthread_detach(philo[i].routine))
			return (0);
		if (pthread_create(&philo[i].monitor, NULL, monitor, &philo[i])
			|| pthread_detach(philo[i].monitor))
			return (0);
		i++;
	}
	if (pthread_mutex_lock(&data->dining))
		return (0);
	return (1);
}

int	main(int argc, char **argv)
{
	t_data	data;
	t_philo	*philo;

	if (!(argc == 5 || argc == 6) || ft_modified_atoi(argv[1]) <= 0
		|| ft_modified_atoi(argv[2]) <= 0 || ft_modified_atoi(argv[3]) <= 0
		|| ft_modified_atoi(argv[4]) <= 0
		|| (argc == 6 && ft_modified_atoi(argv[5]) == 0))
	{
		err_msg();
		return (1);
	}
	data = (t_data){0, };
	init_data1(argc, argv, &data);
	init_data2(argc, argv, &data);
	if (!init_data1(argc, argv, &data) || !init_data2(argc, argv, &data))
		destroy_mutex(&data);
	philo = ft_calloc(data.philo_num, sizeof(t_philo));
	if (philo == NULL)
		destroy_mutex(&data);
	feed_philo(&data, philo);
	if (!feed_philo(&data, philo))
		destroy_mutex(&data);
	return (0);
}
