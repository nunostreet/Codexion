#ifndef CODEXION_H
#define CODEXION_H

#include <stdbool.h>
#include <pthread.h> // mutex: init destroy lock unlock
// threads: create join detach
#include <stdio.h> // printf
#include <stdlib.h> // malloc, free
#include <unistd.h> // write, usleep
#include <stdbool.h>
#include <sys/time.h>
#include <limits.h>

/* ANSI escape sequences for bold text colors
Need to reset color after setting it, with RST
Usage is: printf(R "This is red text." RST) */

# define RST            "\033[0m"
# define BOLD_RED       "\033[1;31m"
# define BOLD_GREEN     "\033[1;32m"
# define BOLD_YELLOW    "\033[1;33m"
# define BOLD_BLUE      "\033[1;34m"
# define BOLD_MAGENTA   "\033[1;35m"
# define BOLD_CYAN      "\033[1;36m"
# define BOLD_WHITE     "\033[1;37m"


typedef pthread_mutex_t t_mtx;

typedef struct s_reunion t_table;

typedef struct s_dongle
{
    t_mtx   dongle;
    int     dongle_id;
}   t_dongle;

/* 
* CODER
*
* ./codexion 5 800 200 200 200 [edf/fifo]
*/

typedef struct s_coder
{
    int         id;
    long        compile_counter;
    bool        compile;
    long        last_compile_time; // time passed since last compile
    t_dongle    *left_dongle;
    t_dongle    *right_dongle;
    pthread_t   thread_id; // a coder is a thread
    t_reunion   *reunion;
}   t_coder;

/* 
* TABLE to keep track of everything
*/

typedef struct s_reunion 
{
    long    number_of_coders; // 👨🏻‍💻
    long    time_to_burnout;
    long    time_to_compile;
    long    time_to_debug;
    long    time_to_refactor;
    long    number_of_comiples_required;
    long    dongle_cooldown;
    long    start_simulation; // start timer ⏱️
    char    *scheduler; // edf or fifo
    bool    end_simulation; // coder burns out 🔥
}  t_reunion;

// *** prototypes ***

// ** utils **
void    error_exit(const char *error);

#endif