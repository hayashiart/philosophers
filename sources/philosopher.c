/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slin <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 09:33:32 by slin              #+#    #+#             */
/*   Updated: 2022/09/08 09:33:34 by slin             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	sleep_routine(t_table *table, time_t sleep_time)
{
	time_t	wake_up;

	wake_up = get_time_in_ms() + sleep_time;
	while (get_time_in_ms() < wake_up)
	{
		if (check_if_flag_end(table) == true)
			break ;
		usleep(100);
	}
}

static void	*solo_philo_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_locks[philo->id]);
	print_status(philo, false, GOT_FORK_1);
	sleep_routine(philo->table, philo->table->t_die);
	print_status(philo, false, DIED);
	pthread_mutex_unlock(&philo->table->fork_locks[philo->id]);
	return (NULL);
}

static	bool	eat_routine(t_philo *philo)
{
	philo->fork[0] = philo->id;
	philo->fork[1] = (philo->id + 1) % philo->table->nb_philos;
	pthread_mutex_lock(&philo->table->fork_locks[philo->fork[0]]);
	print_status(philo, false, GOT_FORK_1);
	pthread_mutex_lock(&philo->table->fork_locks[philo->fork[1]]);
	print_status(philo, false, GOT_FORK_2);
	print_status(philo, false, EATING);
	pthread_mutex_lock(&philo->meal_time_lock);
	philo->last_meal = get_time_in_ms();
	pthread_mutex_unlock(&philo->meal_time_lock);
	sleep_routine(philo->table, philo->table->t_eat);
	if (check_if_flag_end(philo->table) == false)
	{
		pthread_mutex_lock(&philo->meal_time_lock);
		philo->times_ate += 1;
		pthread_mutex_unlock(&philo->meal_time_lock);
	}
	pthread_mutex_unlock(&philo->table->fork_locks[philo->fork[1]]);
	pthread_mutex_unlock(&philo->table->fork_locks[philo->fork[0]]);
	if (check_if_flag_end(philo->table) == true)
		return (false);
	return (true);
}

void	*philo_routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	pthread_mutex_lock(&philo->meal_time_lock);
	philo->last_meal = philo->table->start_time;
	pthread_mutex_unlock(&philo->meal_time_lock);
	sim_start_delay(philo->table->start_time);
	if (philo->table->nb_philos == 1)
		return (solo_philo_routine(philo));
	else if (philo->id % 2 != 0)
		sleep_routine(philo->table, philo->table->t_eat);
	while (check_if_flag_end(philo->table) == false)
	{
		if (eat_routine(philo) == false)
			break ;
		print_status(philo, false, SLEEPING);
		sleep_routine(philo->table, philo->table->t_sleep);
		print_status(philo, false, THINKING);
	}
	return (NULL);
}
