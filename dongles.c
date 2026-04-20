/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nunostreet <nunostreet@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 14:55:01 by nunostreet        #+#    #+#             */
/*   Updated: 2026/04/17 15:45:42 by nunostreet       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/* Fills a t_req according to the chosen scheduler (fifo or edf). */
static t_req	build_request(t_dongle *d, t_coder *coder)
{
	t_req	req;
	long	last;

	req.coder_idx = coder->id - 1;
	req.ticket = d->seq_counter++;
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
static void	wait_for_grant(t_dongle *d, t_coder *coder, t_req req)
{
	while (!simulation_has_ended(coder->reunion))
	{
		if (heap_peek(d->queue, d->queue_size).coder_idx == req.coder_idx
			&& !d->occupied)
			break ;
		pthread_cond_wait(&d->condition, &d->mutex);
	}
}

/* Enqueues a request and blocks until the dongle is granted to this coder. */
void	request_dongle(t_dongle *d, t_coder *coder)
{
	t_req	req;

	safe_mutex_handle(&d->mutex, LOCK);
	req = build_request(d, coder);
	heap_push(d->queue, &d->queue_size, req);
	wait_for_grant(d, coder, req);
	if (!simulation_has_ended(coder->reunion))
		d->occupied = TRUE;
	safe_mutex_handle(&d->mutex, UNLOCK);
}

/* Pops this coder's request, sleeps for cooldown, then releases and wakes waiters. */
void	release_dongle(t_dongle *d, long cooldown)
{
	safe_mutex_handle(&d->mutex, LOCK);
	heap_pop(d->queue, &d->queue_size);
	safe_mutex_handle(&d->mutex, UNLOCK);
	ft_sleep_ms(cooldown);
	safe_mutex_handle(&d->mutex, LOCK);
	d->occupied = FALSE;
	pthread_cond_broadcast(&d->condition);
	safe_mutex_handle(&d->mutex, UNLOCK);
}
