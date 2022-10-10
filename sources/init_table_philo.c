/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_table_philo.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slin <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 02:37:57 by slin              #+#    #+#             */
/*   Updated: 2022/09/08 02:37:59 by slin             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static pthread_mutex_t	*init_fork_locks(t_table *table)
{
	pthread_mutex_t	*fork_locks;
	unsigned int	i;

	fork_locks = malloc(sizeof(pthread_mutex_t) * table->nb_philos);
	if (!fork_locks)
	{
		printf("Error with malloc of forks\n");
		return (NULL);
	}
	i = 0;
	while (i < table->nb_philos)
	{
		if (pthread_mutex_init(&fork_locks[i], 0) != 0)
		{
			printf("Error while initialize fork_locks\n");
			return (NULL);
		}
		i++;
	}
	return (fork_locks);
}

static bool	init_mutex(t_table *table)
{
	if (pthread_mutex_init(&table->end_lock, 0) != 0)
	{
		printf("Error with mutex init end_lock\n");
		return (false);
	}
	if (pthread_mutex_init(&table->write_lock, 0) != 0)
	{
		printf("Error with mutex init write_lock\n ");
		return (false);
	}
	return (true);
}

t_table	*init_table(int argc, char **argv)
{
	t_table	*table;

	table = malloc(sizeof(t_table));
	if (!table)
	{
		printf("Error with table malloc\n");
		return (NULL);
	}
	table->nb_philos = ft_atoi(argv[1]);
	table->t_die = ft_atoi(argv[2]);
	table->t_eat = ft_atoi(argv[3]);
	table->t_sleep = ft_atoi(argv[4]);
	table->nb_meals = -1;
	if (argc - 1 == 5)
		table->nb_meals = ft_atoi(argv[5]);
	table->philos = init_philosophers(table);
	if (!table->philos)
		return (NULL);
	table->fork_locks = init_fork_locks(table);
	if (!table->fork_locks)
		return (NULL);
	if (!init_mutex(table))
		return (NULL);
	table->end = false;
	return (table);
}

static bool	init_values_philo(t_philo *philo, t_table *table, unsigned int i)
{
	if (pthread_mutex_init(&philo->meal_time_lock, 0) != 0)
	{
		printf("Error with mutex for meal_time_lock\n");
		return (false);
	}
	philo->table = table;
	philo->id = i;
	philo->times_ate = 0;
	philo->last_meal = 0;
	return (true);
}

t_philo	**init_philosophers(t_table *table)
{
	t_philo			**philos;
	unsigned int	i;

	philos = malloc(sizeof(t_philo) * table->nb_philos);
	if (!philos)
	{
		printf("Error in malloc of philos\n");
		return (NULL);
	}
	i = 0;
	while (i < table->nb_philos)
	{
		philos[i] = malloc(sizeof(t_philo));
		if (!philos[i])
		{
			printf("Error in malloc of philos[i]\n");
			return (NULL);
		}
		if (!init_values_philo(philos[i], table, i))
			return (NULL);
		i++;
	}
	return (philos);
}
