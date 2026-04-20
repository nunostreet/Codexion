/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nunostreet <nunostreet@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 15:48:44 by nunostreet        #+#    #+#             */
/*   Updated: 2026/04/17 15:48:49 by nunostreet       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/* Swaps two adjacent entries in the heap array. */
static void	swap_entries(t_req *a, t_req *b)
{
	t_req	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

/* Returns 1 if a should be served before b under the current scheduler. */
static int	beats(t_req a, t_req b)
{
	if (a.priority < b.priority)
		return (1);
	if (a.priority > b.priority)
		return (0);
	if (a.tie < b.tie)
		return (1);
	if (a.tie > b.tie)
		return (0);
	return (a.ticket < b.ticket);
}

/* Adds a new request; swaps with top if it has higher priority. */
void	heap_push(t_req *heap, int *size, t_req elem)
{
	heap[*size] = elem;
	(*size)++;
	if (*size == 2 && beats(heap[1], heap[0]))
		swap_entries(&heap[0], &heap[1]);
}

/* Returns the highest-priority request without removing it. */
t_req	heap_peek(t_req *heap, int size)
{
	t_req	empty;

	if (size == 0)
	{
		empty.coder_idx = -1;
		empty.priority = 0;
		empty.ticket = 0;
		empty.tie = 0;
		return (empty);
	}
	return (heap[0]);
}

/* Removes and returns the top request; moves second entry to top if present. */
t_req	heap_pop(t_req *heap, int *size)
{
	t_req	top;

	top = heap[0];
	(*size)--;
	if (*size == 1)
		heap[0] = heap[1];
	return (top);
}
