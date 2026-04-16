#include "codexion.h"

void	wait_all_threads(t_reunion *reunion)
{
	while (!get_bool(&reunion->state_mutex, &reunion->all_threads_ready))
		;
}
