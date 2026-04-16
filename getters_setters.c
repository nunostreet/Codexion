#include "codexion.h"

/* Model to avoid writing LOCK / UNLOCK everywhere */
void	set_bool(t_mtx *mutex, t_bool *dest, t_bool value)
{
	safe_mutex_handle(mutex, LOCK);
	*dest = value;
	safe_mutex_handle(mutex, UNLOCK);
}

t_bool	get_bool(t_mtx *mutex, t_bool *value)
{
	t_bool	result;

	safe_mutex_handle(mutex, LOCK);
	result = *value;
	safe_mutex_handle(mutex, UNLOCK);
	return (result);
}

void	set_long(t_mtx *mutex, long *dest, long value)
{
	safe_mutex_handle(mutex, LOCK);
	*dest = value;
	safe_mutex_handle(mutex, UNLOCK);
}

long	get_long(t_mtx *mutex, long *value)
{
	long	result;

	safe_mutex_handle(mutex, LOCK);
	result = *value;
	safe_mutex_handle(mutex, UNLOCK);
	return (result);
}

void	increment_long(t_mtx *mutex, long *value)
{
	safe_mutex_handle(mutex, LOCK);
	(*value)++;
	safe_mutex_handle(mutex, UNLOCK);
}
