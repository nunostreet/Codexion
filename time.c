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

void	precise_usleep(long usec, t_reunion *reunion)
{
    long	start;
    long	elapsed;
    long	rem;

    start = get_time_ms();
	while (get_time_ms() - start < usec)
	{
		// if simulation finished, break
		elapsed = get_time_ms() - start;
		rem = usec - elapsed;

		if (rem > )
	}


}