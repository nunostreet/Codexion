/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nunostreet <nunostreet@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 22:40:00 by nunostreet        #+#    #+#             */
/*   Updated: 2026/04/17 14:15:35 by nunostreet       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_bool	simulation_has_ended(t_reunion *reunion)
{
	return (get_bool(&reunion->mutexes.state, &reunion->end_simulation));
}

void	*code_simulation(void *data)
{
	t_coder	*coder;

	coder = (t_coder *)data;
	wait_all_threads(coder->reunion);
	while (!simulation_has_ended(coder->reunion))
	{
		if (get_bool(&coder->reunion->mutexes.state, &coder->finished))
			break ;
		grab_dongles(coder);
		if (coder->left_dongle == coder->right_dongle)
		{
			precise_sleep(coder->reunion, coder->reunion->time_to_burnout);
			drop_dongles(coder);
			return (NULL);
		}
		compile(coder);
		drop_dongles(coder);
		if (get_bool(&coder->reunion->mutexes.state, &coder->finished))
			break ;
		debug(coder);
		refactor(coder);
	}
	return (NULL);
}

void	start_simulation(t_reunion *reunion)
{
	int	i;

	if (reunion->number_of_compiles_required == 0)
		return ;
	i = 0;
	while (i < reunion->number_of_coders)
	{
		safe_thread_handle(&reunion->coders[i].thread_id,
			code_simulation, &reunion->coders[i], CREATE);
		i++;
	}
	safe_thread_handle(&reunion->monitor_thread,
		monitor_simulation, reunion, CREATE);
	reunion->start_simulation = get_time_ms();
	set_bool(&reunion->mutexes.state, &reunion->all_threads_ready, TRUE);
	i = 0;
	while (i < reunion->number_of_coders)
	{
		safe_thread_handle(&reunion->coders[i].thread_id, NULL, NULL, JOIN);
		i++;
	}
	safe_thread_handle(&reunion->monitor_thread, NULL, NULL, JOIN);
}
