/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nunostreet <nunostreet@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 16:21:52 by nstreet-          #+#    #+#             */
/*   Updated: 2026/04/16 17:22:57 by nunostreet       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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
/*     printf("coders: %ld\n", reunion.number_of_coders);
	printf("burnout: %ld\n", reunion.time_to_burnout);
	printf("compile: %ld\n", reunion.time_to_compile);
	printf("debug: %ld\n", reunion.time_to_debug);
	printf("refactor: %ld\n", reunion.time_to_refactor);
	printf("compiles required: %ld\n", reunion.number_of_compiles_required);
	printf("cooldown: %ld\n", reunion.dongle_cooldown);
	printf("scheduler: %s\n", reunion.scheduler); */
