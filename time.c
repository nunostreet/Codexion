/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nunostreet <nunostreet@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 22:40:00 by nunostreet        #+#    #+#             */
/*   Updated: 2026/04/20 17:05:31 by nunostreet       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/* Returns the current time in milliseconds since the Unix epoch. */
long	get_time_ms(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		error_exit("Get time of day failed.");
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
}

/* Returns milliseconds elapsed since start. */
long	elapsed_ms(long start)
{
	return (get_time_ms() - start);
}

/* Sleeps for the given number of milliseconds using nanosleep. */
void	ft_sleep_ms(long ms)
{
	struct timespec	ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	nanosleep(&ts, NULL);
}

/* Sleeps for duration ms, checking every 1-5ms to avoid overshooting. */
void	precise_sleep(t_reunion *reunion, long duration)
{
	long	start;
	long	elapsed;
	long	remaining;

	(void)reunion;
	start = get_time_ms();
	while (elapsed_ms(start) < duration)
	{
		elapsed = elapsed_ms(start);
		remaining = duration - elapsed;
		if (remaining > 10)
			ft_sleep_ms(5);
		else
			ft_sleep_ms(1);
	}
}
