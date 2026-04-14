#include "codexion.h"

/* Convert current time to milliseconds (Unix epoch)*/
long	get_time_ms(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
}

long	elapsed_ms(long start)
{
	return (get_time_ms() - start);
}
