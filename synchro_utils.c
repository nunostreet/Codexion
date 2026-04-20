/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   synchro_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nunostreet <nunostreet@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 22:40:00 by nunostreet        #+#    #+#             */
/*   Updated: 2026/04/17 15:18:53 by nunostreet       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/* Prints a timestamped state line, serialised by the write mutex. */
void	print_state(t_coder *coder, const char *msg)
{
	safe_mutex_handle(&coder->reunion->mutexes.write, LOCK);
	if (!simulation_has_ended(coder->reunion))
		printf("%ld %d %s\n",
			elapsed_ms(coder->reunion->start_simulation), coder->id, msg);
	safe_mutex_handle(&coder->reunion->mutexes.write, UNLOCK);
}

/* Spins until the start barrier is released by start_simulation. */
void	wait_all_threads(t_reunion *reunion)
{
	while (!get_bool(&reunion->mutexes.state, &reunion->all_threads_ready))
		;
}

/* Destroys all dongles and frees the coder and dongle arrays. */
void	cleanup_reunion(t_reunion *reunion)
{
	cleanup_dongles(reunion, reunion->number_of_coders);
	free(reunion->coders);
	free(reunion->dongles);
}

/* Destroys mutex and cond for each initialised dongle, then global mutexes. */
void	cleanup_dongles(t_reunion *reunion, int count)
{
	while (--count >= 0)
	{
		pthread_cond_destroy(&reunion->dongles[count].condition);
		safe_mutex_handle(&reunion->dongles[count].mutex, DESTROY);
	}
	safe_mutex_handle(&reunion->mutexes.write, DESTROY);
	safe_mutex_handle(&reunion->mutexes.state, DESTROY);
}
