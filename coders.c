/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nunostreet <nunostreet@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 17:14:01 by nunostreet        #+#    #+#             */
/*   Updated: 2026/04/17 13:04:48 by nunostreet       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/* Acquires both dongles in dongle_id order to prevent deadlock. */
void	grab_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	if (coder->left_dongle->dongle_id < coder->right_dongle->dongle_id)
	{
		first = coder->left_dongle;
		second = coder->right_dongle;
	}
	else
	{
		first = coder->right_dongle;
		second = coder->left_dongle;
	}
	request_dongle(first, coder);
	print_state(coder, "has taken a dongle");
	request_dongle(second, coder);
	print_state(coder, "has taken a dongle");
}

/* Releases both dongles with cooldown in reverse acquisition order. */
void	drop_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;
	long		cooldown;

	cooldown = coder->reunion->dongle_cooldown;
	if (coder->left_dongle->dongle_id < coder->right_dongle->dongle_id)
	{
		first = coder->left_dongle;
		second = coder->right_dongle;
	}
	else
	{
		first = coder->right_dongle;
		second = coder->left_dongle;
	}
	release_dongle(second, cooldown);
	release_dongle(first, cooldown);
}

/* Prints state and sleeps for the debug phase duration. */
void	debug(t_coder *coder)
{
	print_state(coder, "is debugging");
	precise_sleep(coder->reunion->time_to_debug);
}

/* Resets the burnout timer, increments counter, sleeps for compile duration. */
void	compile(t_coder *coder)
{
	set_long(&coder->reunion->mutexes.state, &coder->last_compile_start,
		get_time_ms());
	increment_long(&coder->reunion->mutexes.state, &coder->compile_counter);
	print_state(coder, "is compiling");
	precise_sleep(coder->reunion->time_to_compile);
	if (get_long(&coder->reunion->mutexes.state, &coder->compile_counter)
		>= coder->reunion->number_of_compiles_required)
		set_bool(&coder->reunion->mutexes.state, &coder->finished, TRUE);
}

/* Prints state and sleeps for the refactor phase duration. */
void	refactor(t_coder *coder)
{
	print_state(coder, "is refactoring");
	precise_sleep(coder->reunion->time_to_refactor);
}
