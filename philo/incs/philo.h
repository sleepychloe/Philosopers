/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhwang <yhwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 23:48:58 by yhwang            #+#    #+#             */
/*   Updated: 2022/05/15 23:56:47 by yhwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <string.h>

enum e_state
{
	ALIVE,
	DEAD,
	COMPLETE
};

typedef struct s_data
{
	int					philo_num;
	int					time_die;
	int					time_eat;
	int					time_sleep;
	int					num_must_eat;
	int					state;
	pthread_mutex_t		*fork;
	pthread_mutex_t		changing_state;
	pthread_mutex_t		printing;
	size_t				time_start;
}		t_data;

typedef struct s_philo
{
	int					id;
	size_t				time_last;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	pthread_mutex_t		dining;
	t_data				*data;
}		t_philo;

int		ft_modified_atoi(const char *str);
void	ft_free(int flag, t_data *data, t_philo *philo,
			pthread_t *thread_philo);
void	err_msg(void);
size_t	get_time_mili(void);
size_t	get_time_micro(void);
void	fy_usleep(size_t timecheck);
int		init_data_fy_norminette(t_data *data);
int		init_data(t_data *data, int argc, char **argv);
void	philo_print(t_philo *philo, const char *string);
int		monitoring(t_philo *philo, t_data *data);
int		death_checker(t_philo *philo);
void	philo_state(t_philo *philo);
void	philo_eat(t_philo *philo, int *cnt_eat);
int		take_two_forks(t_philo *philo);
void	*philo_start(void *param);
int		init_philo(t_philo *philo, t_data *data, int i);
void	philo_num_is_one(t_data *data);
int		setting_up_philo(t_data *data, t_philo *philo, pthread_t *thread_philo);
int		main(int argc, char **argv);

#endif
