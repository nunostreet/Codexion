/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nunostreet <nunostreet@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 14:55:01 by nunostreet        #+#    #+#             */
/*   Updated: 2026/04/22 23:59:48 by nunostreet       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/* Fills a t_req according to the chosen scheduler (fifo or edf). */
static t_req	build_request(t_dongle *dongle, t_coder *coder)
{
	t_req	req;
	long	last;

	req.coder_idx = coder->id - 1;
	req.ticket = dongle->seq_counter++;
	if (strcmp(coder->reunion->scheduler, "edf") == 0)
	{
		last = get_long(&coder->reunion->mutexes.state,
				&coder->last_compile_start);
		req.priority = last + coder->reunion->time_to_burnout;
		req.tie = req.ticket;
	}
	else
	{
		req.priority = 0;
		req.tie = 0;
	}
	return (req);
}

/* Blocks on cond_wait until this coder is at the top of the queue. */
static void	wait_for_grant(t_dongle *dongle, t_coder *coder, t_req req)
{
	long	remaining;

	while (!simulation_has_ended(coder->reunion))
	{
		if (heap_peek(dongle->queue, dongle->queue_size).coder_idx
			== req.coder_idx && !dongle->occupied)
		{
			remaining = dongle->available_at - get_time_ms();
			if (remaining <= 0)
				break ;
			safe_mutex_handle(&dongle->mutex, UNLOCK);
			ft_sleep_ms(remaining);
			safe_mutex_handle(&dongle->mutex, LOCK);
			continue ;
		}
		pthread_cond_wait(&dongle->condition, &dongle->mutex);
	}
}

/* Enqueues a request and blocks until the dongle is granted to this coder. */
void	request_dongle(t_dongle *dongle, t_coder *coder)
{
	t_req	req;

	safe_mutex_handle(&dongle->mutex, LOCK);
	req = build_request(dongle, coder);
	heap_push(dongle->queue, &dongle->queue_size, req);
	wait_for_grant(dongle, coder, req);
	heap_pop(dongle->queue, &dongle->queue_size);
	if (!simulation_has_ended(coder->reunion))
		dongle->occupied = TRUE;
	safe_mutex_handle(&dongle->mutex, UNLOCK);
}

/* Pops request, sleeps cooldown, marks dongle free, then wakes waiters. */
void	release_dongle(t_dongle *dongle, long cooldown)
{
	safe_mutex_handle(&dongle->mutex, LOCK);
	dongle->occupied = FALSE;
	dongle->available_at = get_time_ms() + cooldown;
	pthread_cond_broadcast(&dongle->condition);
	safe_mutex_handle(&dongle->mutex, UNLOCK);
}
