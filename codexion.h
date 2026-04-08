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

#endif

typedef pthread_mutex_t t_mtx;

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
    int id;
    long    compile_counter;
    bool    compile;
    long    last_compile_time; // time passed since last compile
    t_dongle    *left_dongle;
    t_dongle    *right_dongle;
    pthread_t   thread_id; // a coder is a thread
}   t_coder;

/* 
 * TABLE to keep track of everything
 */

 typedef struct s_reunion
 {
    long    coder_nbr; // 👨🏻‍💻
    long    time_to_burnout;
    long    time_to_compile;
    long    time_to_sleep;
    long    limit_compilations;
    long    dongle_cooldown;
    long    start_simulation; // start timer ⏱️
    char    *scheduler; // edf or fifo
    bool    end_simulation; // coder burns out 🔥
 }  t_reunion;

 /* Need to check all names */