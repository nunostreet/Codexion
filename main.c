/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nstreet- <nstreet-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 16:21:52 by nstreet-          #+#    #+#             */
/*   Updated: 2026/04/20 18:12:33 by nstreet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/* Parses arguments, initialises the simulation, runs it, and cleans up. */
int	main(int ac, char **av)
{
	t_reunion	reunion;

	if (ac != 9)
		error_exit("Usage: ./codexion n_coders t_burnout t_compile t_debug "
			"t_refactor n_compiles cooldown [fifo|edf]");
	parse_input(&reunion, av);
	reunion.start_simulation = get_time_ms();
	if (init_reunion(&reunion) != 0)
		error_exit("Initialization failed");
	start_simulation(&reunion);
	cleanup_reunion(&reunion);
	return (0);
}
