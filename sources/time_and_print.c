/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slin <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 08:10:57 by slin              #+#    #+#             */
/*   Updated: 2022/09/08 08:10:58 by slin             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

time_t	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	print_status(t_philo *philo, bool report, t_status status)
{
	pthread_mutex_lock(&philo->table->write_lock);
	if (check_if_flag_end(philo->table) == true && report == false)
	{
		pthread_mutex_unlock(&philo->table->write_lock);
		return ;
	}
	if (status == DIED)
		printf("%ld %d died\n", get_time_in_ms()
			- philo->table->start_time, philo->id + 1);
	if (status == EATING)
		printf("%ld %d is eating\n", get_time_in_ms()
			- philo->table->start_time, philo->id + 1);
	if (status == SLEEPING)
		printf("%ld %d is sleeping\n", get_time_in_ms()
			- philo->table->start_time, philo->id + 1);
	if (status == THINKING)
		printf("%ld %d is thinking\n", get_time_in_ms()
			- philo->table->start_time, philo->id + 1);
	if (status == GOT_FORK_1 || status == GOT_FORK_2)
		printf("%ld %d has taken a fork\n", get_time_in_ms()
			- philo->table->start_time, philo->id + 1);
	pthread_mutex_unlock(&philo->table->write_lock);
}

void	sim_start_delay(time_t start_time)
{
	while (get_time_in_ms() < start_time)
		continue ;
}
