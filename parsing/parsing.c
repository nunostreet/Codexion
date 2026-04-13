/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nunostreet <nunostreet@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 16:21:58 by nstreet-          #+#    #+#             */
/*   Updated: 2026/04/10 10:01:21 by nunostreet       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	parse_numeric_args(t_reunion *reunion, char **av)
{
	reunion->number_of_coders = parse_long_arg(av[1], "number_of_coders");
	reunion->time_to_burnout = parse_long_arg(av[2], "time_to_burnout");
	reunion->time_to_compile = parse_long_arg(av[3], "time_to_compile");
	reunion->time_to_debug = parse_long_arg(av[4], "time_to_debug");
	reunion->time_to_refactor = parse_long_arg(av[5], "time_to_refactor");
	reunion->number_of_compiles_required = parse_long_arg(av[6],
			"number_of_compiles_required");
	reunion->dongle_cooldown = parse_long_arg(av[7], "dongle_cooldown");
}

static void	validate_numeric_args(t_reunion *reunion)
{
	if (reunion->number_of_coders <= 0)
		error_field("number_of_coders", "must be greater than 0");
	if (reunion->time_to_burnout < 60)
		error_field("time_to_burnout", "must be at least 60 ms");
	if (reunion->time_to_compile < 60)
		error_field("time_to_compile", "must be at least 60 ms");
	if (reunion->time_to_debug < 60)
		error_field("time_to_debug", "must be at least 60 ms");
	if (reunion->time_to_refactor < 60)
		error_field("time_to_refactor", "must be at least 60 ms");
	if (reunion->number_of_compiles_required <= 0)
		error_field("number_of_compiles_required", "must be greater than 0");
	if (reunion->dongle_cooldown < 60)
		error_field("dongle_cooldown", "must be at least 60 ms");
}

void	parse_input(t_reunion *reunion, char **av)
{
	parse_numeric_args(reunion, av);
	validate_numeric_args(reunion);
	reunion->scheduler = "fifo";
	parse_scheduler(reunion, av[8]);
}
