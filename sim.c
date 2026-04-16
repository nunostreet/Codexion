#include "codexion.h"

t_bool	simulation_finished(t_reunion *reunion)
{
	return (get_bool(&reunion->state_mutex, &reunion->end_simulation));
}

void	*code_simulation(void *data)
{
	t_coder	*coder;

	coder = (t_coder *)data;
	wait_all_threads(coder->reunion);
	while (!simulation_has_ended(coder->reunion) && !coder->finished)
	{
		if (coder->all_compiled)
			break;

		compile(coder);

		// write_status & precise usleep

		debug(coder);
	}
	return (NULL);
}

void	start_simulation(t_reunion *reunion)
{
	int	i;

	i = 0;
	if (reunion->number_of_compiles_required == 0)
		return;
	else if (reunion->number_of_coders == 1)
		;// TODO;
	else
	{
		while (i++ < reunion->number_of_coders)
			safe_thread_handle(&reunion->coders[i].thread_id, code_simulation, 
				&reunion->coders[i], CREATE);
	}
	reunion->start_simulation = get_time_ms();
	set_bool(&reunion->state_mutex, &reunion->all_threads_ready, true);
	i = -1;
	while (++i < reunion->number_of_coders)
		safe_thread_handle(&reunion->coders[i].thread_id, NULL, NULL, JOIN);
}
