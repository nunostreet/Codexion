/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nunostreet <nunostreet@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 13:56:32 by nunostreet        #+#    #+#             */
/*   Updated: 2026/04/17 14:33:17 by nunostreet       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/* Sets end_simulation to TRUE to signal all threads to stop. */
void	stop_simulation(t_reunion *reunion)
{
	set_bool(&reunion->mutexes.state, &reunion->end_simulation, TRUE);
}

static t_bool	coder_burned_out(t_coder *coder)
{
	long	last_compile;

	last_compile = get_long(&coder->reunion->mutexes.state,
			&coder->last_compile_start);
	if (elapsed_ms(last_compile) >= coder->reunion->time_to_burnout)
		return (TRUE);
	return (FALSE);
}

static t_bool	all_coders_finished(t_reunion *reunion)
{
	int	i;

	i = 0;
	while (i < reunion->number_of_coders)
	{
		if (!get_bool(&reunion->mutexes.state, &reunion->coders[i].finished))
			return (FALSE);
		i++;
	}
	return (TRUE);
}

static t_bool	burnout_detected(t_reunion *reunion)
{
	int			i;

	i = 0;
	while (i < reunion->number_of_coders)
	{
		if (!get_bool(&reunion->mutexes.state, &reunion->coders[i].finished)
			&& coder_burned_out(&reunion->coders[i]))
		{
			print_state(&reunion->coders[i], "burned out");
			stop_simulation(reunion);
			return (TRUE);
		}
		i++;
	}
	return (FALSE);
}

/* Monitor thread: checks for burnout every 1ms and stops the simulation. */
void	*monitor_simulation(void *data)
{
	t_reunion	*reunion;

	reunion = (t_reunion *)data;
	wait_all_threads(reunion);
	while (!simulation_has_ended(reunion))
	{
		if (burnout_detected(reunion))
			return (NULL);
		if (all_coders_finished(reunion))
		{
			stop_simulation(reunion);
			return (NULL);
		}
		ft_sleep_ms(1);
	}
	return (NULL);
}
