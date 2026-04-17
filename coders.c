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

void	grab_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	if (coder->left_dongle == coder->right_dongle)
	{
		safe_mutex_handle(&coder->left_dongle->mutex, LOCK);
		print_state(coder, "has taken a dongle");
		return ;
	}
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
	safe_mutex_handle(&first->mutex, LOCK);
	print_state(coder, "has taken a dongle");
	safe_mutex_handle(&second->mutex, LOCK);
	print_state(coder, "has taken a dongle");
}

void	debug(t_coder *coder)
{
	print_state(coder, "is debugging");
	precise_sleep(coder->reunion, coder->reunion->time_to_debug);
}

void	compile(t_coder *coder)
{
	increment_long(&coder->reunion->mutexes.state, &coder->compile_counter);
	print_state(coder, "is compiling");
	set_long(&coder->reunion->mutexes.state, &coder->last_compile_start,
		get_time_ms());
	precise_sleep(coder->reunion, coder->reunion->time_to_compile);
	if (get_long(&coder->reunion->mutexes.state, &coder->compile_counter)
		>= coder->reunion->number_of_compiles_required)
		set_bool(&coder->reunion->mutexes.state, &coder->finished, TRUE);
}

void	refactor(t_coder *coder)
{
	print_state(coder, "is refactoring");
	precise_sleep(coder->reunion, coder->reunion->time_to_refactor);
}

void	drop_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	if (coder->left_dongle == coder->right_dongle)
	{
		safe_mutex_handle(&coder->left_dongle->mutex, UNLOCK);
		return ;
	}
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
	safe_mutex_handle(&second->mutex, UNLOCK);
	safe_mutex_handle(&first->mutex, UNLOCK);
}
