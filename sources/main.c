/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slin <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 03:03:03 by slin              #+#    #+#             */
/*   Updated: 2022/09/06 03:03:05 by slin             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static bool	contain_only_digit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
		i++;
	}
	return (true);
}

int	ft_atoi(char *str)
{
	int	i;
	int	res;

	i = 0;
	res = 0;
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		res = (res * 10) + str[i] - '0';
		i++;
	}
	return (res);
}

static int	check_arg2(int i, int nb)
{
	if (i == 1 && (nb <= 0 || nb > MAX_PHILOS))
	{
		printf("Error with number of philosophers\n");
		exit(1);
	}
	if (i != 1 && ((nb <= 0) || (nb > 2147483647)))
	{
		printf("Error with argument\n");
		exit(1);
	}
	return (0);
}

static	int	check_arg(int argc, char **argv)
{
	int	i;
	int	nb;

	i = 1;
	if (((argc - 1) != 4) && ((argc - 1) != 5))
	{
		printf("Error with number of arguments\n");
		exit(1);
	}
	while (i < argc)
	{
		if (contain_only_digit(argv[i]) == false)
		{
			printf("Error with the arguments\n");
			exit(1);
		}
		nb = ft_atoi(argv[i]);
		check_arg2(i, nb);
		i++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	unsigned int	i;
	t_table			*table;

	check_arg(argc, argv);
	table = init_table(argc, argv);
	if (!table)
		return (1);
	if (!start_simulation(table))
		return (1);
	i = 0;
	while (i < table->nb_philos)
	{
		pthread_join(table->philos[i]->thread, NULL);
		i++;
	}
	if (table->nb_philos > 1)
		pthread_join(table->reaper, NULL);
	destroy_mutexes(table);
	free_table(table);
	return (0);
}
