/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_scheduler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nstreet- <nstreet-@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 17:23:00 by nstreet-          #+#    #+#             */
/*   Updated: 2026/04/09 17:45:00 by nstreet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/* Compares two strings lexicographically; returns 0 if equal. */
static int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s1 == *s2)
	{
		++s1;
		++s2;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

/* Sets the scheduler to "edf" or "fifo", or exits on invalid input. */
void	parse_scheduler(t_reunion *reunion, char *arg)
{
	if (ft_strcmp(arg, "edf") == 0)
		reunion->scheduler = "edf";
	else if (ft_strcmp(arg, "fifo") == 0)
		reunion->scheduler = "fifo";
	else
		error_field("scheduler", "must be 'edf' or 'fifo'");
}
