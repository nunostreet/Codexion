#include "codexion.h"

/*
 * ./codexion 5 200 200 200 200 5 200 [edf/fifo]
 */

int main(int ac, char **av)
{
    t_reunion reunion;

    if (ac == 9)
    {
        // Error checking
        parse_input(&reunion, av);
        // Creating the actual thing
        data_init(&reunion);
        compilation_start(&reunion);
        // If all compiles done or burnout happened
        clean(&reunion);
    }
    else
    {
        error_exit("Wrong input:\n"
            BOLD_GREEN"Correct usage is: ./codexion 5 200 200 200 200 5 200 [edf/fifo]"RST);
    }
}