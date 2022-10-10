/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grim_reaper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slin <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 14:54:20 by slin              #+#    #+#             */
/*   Updated: 2022/09/08 14:54:22 by slin             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool	check_if_flag_end(t_table *table)
{
	bool	res;

	res = false;
	pthread_mutex_lock(&table->end_lock);
	if (table->end == true)
		res = true;
	pthread_mutex_unlock(&table->end_lock);
	return (res);
}

static void	set_end_flag(t_table *table, bool state)
{
	pthread_mutex_lock(&table->end_lock);
	table->end = state;
	pthread_mutex_unlock(&table->end_lock);
}

static bool	is_philo_dead(t_philo *philo)
{
	time_t	time;

	pthread_mutex_lock(&philo->table->write_lock);
	time = get_time_in_ms();
	pthread_mutex_unlock(&philo->table->write_lock);
	if ((time - philo->last_meal) >= philo->table->t_die)
	{
		print_status(philo, true, DIED);
		set_end_flag(philo->table, true);
		pthread_mutex_unlock(&philo->meal_time_lock);
		return (true);
	}
	return (false);
}

static bool	end_condition_reached(t_table *table)
{
	unsigned int	i;
	bool			all_ate_enough;

	all_ate_enough = true;
	i = 0;
	while (i < table->nb_philos)
	{
		pthread_mutex_lock(&table->philos[i]->meal_time_lock);
		if (is_philo_dead(table->philos[i]))
			return (true);
		if (table->nb_meals != -1)
			if (table->philos[i]->times_ate < (unsigned int)table->nb_meals)
				all_ate_enough = false;
		pthread_mutex_unlock(&table->philos[i]->meal_time_lock);
		i++;
	}
	if (table->nb_meals != -1 && all_ate_enough == true)
	{
		set_end_flag(table, true);
		return (true);
	}
	return (false);
}

void	*reaper_routine(void *data)
{
	t_table	*table;

	table = (t_table *)data;
	set_end_flag(table, false);
	sim_start_delay(table->start_time);
	while (true)
	{
		if (end_condition_reached(table) == true)
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
