/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   synchro_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nunostreet <nunostreet@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 22:40:00 by nunostreet        #+#    #+#             */
/*   Updated: 2026/04/16 22:40:00 by nunostreet       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_state(t_coder *coder, const char *msg)
{
	safe_mutex_handle(&coder->reunion->mutexes.write, LOCK);
	if (!simulation_has_ended(coder->reunion))
		printf("%ld %d %s\n",
			elapsed_ms(coder->reunion->start_simulation), coder->id, msg);
	safe_mutex_handle(&coder->reunion->mutexes.write, UNLOCK);
}

void	wait_all_threads(t_reunion *reunion)
{
	while (!get_bool(&reunion->mutexes.state, &reunion->all_threads_ready))
		;
}

void	cleanup_reunion(t_reunion *reunion)
{
	int	i;

	i = 0;
	while (i < reunion->number_of_coders)
	{
		safe_mutex_handle(&reunion->dongles[i].mutex, DESTROY);
		i++;
	}
	safe_mutex_handle(&reunion->mutexes.write, DESTROY);
	safe_mutex_handle(&reunion->mutexes.state, DESTROY);
	free(reunion->coders);
	free(reunion->dongles);
}
