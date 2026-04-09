#include "codexion.h"

static inline bool ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

static inline bool ft_isspace(int c)
{
	return ((c >= 9 && c <= 13) || c == ' ');
}

/* 
 * 1) check for negatives
 * 2) check if number is legit
 * 3) check for INT_MAX
 */
static const char   *valid_input(const char *str)
{
    int         len;
    const char  *number;

    len = 0;
    while(ft_isspace(*str))
        ++str;
    if (*str == '+')
        ++str;
    else if (*str == '-')
        error_exit("Value needs to be positive.");
    if (!ft_isdigit(*str))
        error_exit("The input is not a correcct digit.");
    number = str;
    while (ft_isdigit(*str++))
        ++len;
    if (len > 10)
        error_exit("The value is too big, INT_MAX is the limit.");
    return (number);
}

static long ft_atol(const char *str)
{
    long    num;

    num = 0;
    str = valid_input(str);
    while (is_digit(*str))
        num = num * 10 + (*str++ - '0');
    if (num > INT_MAX)
        error_exit("The value is too big. INT_MAX is the limit.");
    return (num);
}

/*
 * ./codexion 5 200 200 200 200 5 200 [edf/fifo]
 *
 * av[1] = number_of_coders
 * av[2] = time_to_burnout
 * av[3] = time_to_compile
 * av[4] = time_to_debug
 * av[5] = time_to_refactor
 * av[6] = number_of_compiles_required
 * av[7] = dongle_cooldown
 * av[8] = scheduler (char *)
 *
 * need to check (1) if actual numbers, (2) if not larger than int_max and (3) timestamps > x
 */

void    parse_input(t_reunion *reunion, char **av)
{
    reunion->number_of_coders = ft_atol(av[1]);
    reunion->time_to_burnout = ft_atol(av[2]);
    reunion->time_to_compile = ft_atol(av[3]);
    reunion->time_to_debug = ft_atol(av[4]);
    reunion->time_to_refactor = ft_atol(av[5]);
    reunion->number_of_comiples_required = ft_atol(av[6]);
    reunion->dongle_cooldown = ft_atol(av[7]);
}
