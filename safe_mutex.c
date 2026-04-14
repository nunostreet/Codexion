#include "codexion.h"
#include <errno.h>

static void	handle_invalid_status(t_opcode opcode)
{
	if (opcode == INIT)
		error_exit("Mutex init failed: invalid mutex or parameters");
	error_exit("Mutex operation failed: invalid mutex or parameters");
}

static void	handle_mutex_error(int status, t_opcode opcode)
{
	if (status == 0)
		return ;
	if ((status == EINVAL && (opcode == LOCK || opcode == UNLOCK
				|| opcode == DESTROY))
		|| (status == EINVAL && opcode == INIT))
		handle_invalid_status(opcode);
	if (status == EDEADLK)
		error_exit("Mutex lock failed: deadlock detected");
	if (status == EPERM)
		error_exit("Mutex unlock failed: thread does not own mutex");
	if (status == EBUSY)
		error_exit("Mutex destroy failed: mutex is still locked");
	if (status == ENOMEM)
		error_exit("Mutex init failed: insufficient memory");
	if (status == EAGAIN)
		error_exit("Mutex init failed: system mutex limit reached");
	error_exit("Mutex operation failed");
}

void	safe_mutex_handle(t_mtx *mutex, t_opcode opcode)
{
	int	status;

	status = 0;
	if (opcode == LOCK)
		status = pthread_mutex_lock(mutex);
	else if (opcode == UNLOCK)
		status = pthread_mutex_unlock(mutex);
	else if (opcode == INIT)
		status = pthread_mutex_init(mutex, NULL);
	else if (opcode == DESTROY)
		status = pthread_mutex_destroy(mutex);
	else
		error_exit("Wrong opcode for mutex handle");
	handle_mutex_error(status, opcode);
}
