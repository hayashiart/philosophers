/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slin <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 03:02:47 by slin              #+#    #+#             */
/*   Updated: 2022/09/06 03:02:50 by slin             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <limits.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <stdbool.h>

# define MAX_PHILOS	250

typedef struct s_philo	t_philo;

typedef struct s_table
{
	time_t			start_time;
	unsigned int	nb_philos;
	pthread_t		reaper;
	time_t			t_die;
	time_t			t_eat;
	time_t			t_sleep;
	int				nb_meals;
	bool			end;
	pthread_mutex_t	end_lock;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	*fork_locks;
	t_philo			**philos;
}	t_table;

typedef struct s_philo
{
	pthread_t		thread;
	unsigned int	id;
	unsigned int	times_ate;
	unsigned int	fork[2];
	pthread_mutex_t	meal_time_lock;
	time_t			last_meal;
	t_table			*table;
}	t_philo;

typedef enum e_status
{
	DIED = 0,
	EATING = 1,
	SLEEPING = 2,
	THINKING = 3,
	GOT_FORK_1 = 4,
	GOT_FORK_2 = 5
}	t_status;

int		ft_atoi(char *str);
t_table	*init_table(int argc, char **argv);
t_philo	**init_philosophers(t_table *table);
time_t	get_time_in_ms(void);
void	print_status(t_philo *philo, bool report, t_status status);
bool	start_simulation(t_table *table);
void	*free_table(t_table *table);
void	destroy_mutexes(t_table *table);
void	*philo_routine(void *data);
bool	check_if_flag_end(t_table *table);
void	*reaper_routine(void *data);
void	sim_start_delay(time_t start_time);

#endif