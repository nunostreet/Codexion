/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nunostreet <nunostreet@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 12:50:46 by nunostreet        #+#    #+#             */
/*   Updated: 2026/04/17 15:30:45 by nunostreet       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	alloc_arrays(t_reunion *reunion)
{
	reunion->coders = malloc(sizeof(t_coder) * reunion->number_of_coders);
	if (!reunion->coders)
		return (1);
	reunion->dongles = malloc(sizeof(t_dongle) * reunion->number_of_coders);
	if (!reunion->dongles)
	{
		free(reunion->coders);
		reunion->coders = NULL;
		return (1);
	}
	return (0);
}

static int	init_mutexes(t_reunion *reunion)
{
	if (pthread_mutex_init(&reunion->mutexes.write, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&reunion->mutexes.state, NULL) != 0)
	{
		pthread_mutex_destroy(&reunion->mutexes.write);
		return (1);
	}
	return (0);
}

static int	init_dongles(t_reunion *reunion)
{
	int	i;

	i = 0;
	while (i < reunion->number_of_coders)
	{
		memset(&reunion->dongles[i], 0, sizeof(t_dongle));
		reunion->dongles[i].dongle_id = i + 1;
		if (pthread_mutex_init(&reunion->dongles[i].mutex, NULL))
			return (cleanup_dongles(reunion, i), 1);
		if (pthread_cond_init(&reunion->dongles[i].condition, NULL))
		{
			pthread_mutex_destroy(&reunion->dongles[i].mutex);
			return (cleanup_dongles(reunion, i), 1);
		}
		i++;
	}
	return (0);
}

static void	init_coders(t_reunion *reunion)
{
	int	i;

	i = 0;
	while (i < reunion->number_of_coders)
	{
		reunion->coders[i].id = i + 1;
		reunion->coders[i].compile_counter = 0;
		reunion->coders[i].last_compile_start = reunion->start_simulation;
		reunion->coders[i].finished = FALSE;
		reunion->coders[i].left_dongle = &reunion->dongles[i];
		reunion->coders[i].right_dongle = &reunion->dongles[(i + 1)
			% reunion->number_of_coders];
		reunion->coders[i].reunion = reunion;
		i++;
	}
}

int	init_reunion(t_reunion *reunion)
{
	reunion->coders = NULL;
	reunion->dongles = NULL;
	reunion->end_simulation = FALSE;
	reunion->all_threads_ready = FALSE;
	if (alloc_arrays(reunion))
		return (1);
	if (init_mutexes(reunion))
		return (free(reunion->coders), free(reunion->dongles), 1);
	if (init_dongles(reunion))
		return (free(reunion->coders), free(reunion->dongles), 1);
	init_coders(reunion);
	return (0);
}
