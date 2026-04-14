#include "codexion.h"
#include <errno.h>

static void	handle_mutex_error(int status, t_opcode opcode)
{
	if (status == 0)
		return ;
	if ((status == EINVAL && (opcode == LOCK || opcode == UNLOCK
				|| opcode == DESTROY))
		|| (status == EINVAL && opcode == INIT))
		error_exit("Mutex operation failed: invalid mutex or parameters");
	if (status == EDEADLK)
		error_exit("Mutex lock failed: deadlock detected");
	if (status == EPERM)
		error_exit("Mutex unlock failed: current thread does not own mutex");
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

static void	handle_thread_error(int status, t_opcode opcode)
{
	if (status == 0)
		return ;
	if (status == EAGAIN && opcode == CREATE)
		error_exit("Thread create failed: insufficient resources");
	if (status == EPERM && opcode == DETACH)
		error_exit("Thread detach failed: insufficient permissions");
	if (status == EINVAL && opcode == CREATE)
		error_exit("Thread create failed: invalid attributes");
	if (status == EINVAL && (opcode == JOIN || opcode == DETACH))
		error_exit("Thread operation failed: invalid thread handle");
	if (status == ESRCH)
		error_exit("Thread operation failed: no matching thread");
	if (status == EDEADLK)
		error_exit("Thread join failed: deadlock detected");
	error_exit("Thread operation failed");
}

void	safe_thread_handle(pthread_t *thread, void *(*foo)(void *), void *data,
		t_opcode opcode)
{
	int	status;

	status = 0;
	if (opcode == CREATE)
		status = pthread_create(thread, NULL, foo, data);
	else if (opcode == JOIN)
		status = pthread_join(*thread, NULL);
	else if (opcode == DETACH)
		status = pthread_detach(*thread);
	else
		error_exit("Wrong opcode for thread handle");
	handle_thread_error(status, opcode);
}
