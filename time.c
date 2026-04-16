#include "codexion.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		error_exit("Get time of day failed.");
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
}

long	elapsed_ms(long start)
{
	return (get_time_ms() - start);
}

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
			usleep(1000 * 5);
		else
			usleep(1000);
	}
}
