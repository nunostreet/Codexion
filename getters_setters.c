/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters_setters.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nunostreet <nunostreet@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 22:40:00 by nunostreet        #+#    #+#             */
/*   Updated: 2026/04/16 22:40:00 by nunostreet       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/* Model to avoid writing LOCK / UNLOCK everywhere */
void	set_bool(pthread_mutex_t *mutex, t_bool *dest, t_bool value)
{
	safe_mutex_handle(mutex, LOCK);
	*dest = value;
	safe_mutex_handle(mutex, UNLOCK);
}

t_bool	get_bool(pthread_mutex_t *mutex, t_bool *value)
{
	t_bool	result;

	safe_mutex_handle(mutex, LOCK);
	result = *value;
	safe_mutex_handle(mutex, UNLOCK);
	return (result);
}

void	set_long(pthread_mutex_t *mutex, long *dest, long value)
{
	safe_mutex_handle(mutex, LOCK);
	*dest = value;
	safe_mutex_handle(mutex, UNLOCK);
}

long	get_long(pthread_mutex_t *mutex, long *value)
{
	long	result;

	safe_mutex_handle(mutex, LOCK);
	result = *value;
	safe_mutex_handle(mutex, UNLOCK);
	return (result);
}

void	increment_long(pthread_mutex_t *mutex, long *value)
{
	safe_mutex_handle(mutex, LOCK);
	(*value)++;
	safe_mutex_handle(mutex, UNLOCK);
}
