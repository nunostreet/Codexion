/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nunostreet <nunostreet@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 16:21:43 by nstreet-          #+#    #+#             */
/*   Updated: 2026/04/17 15:28:26 by nunostreet       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <time.h>
# include <unistd.h>

# define RST "\033[0m"
# define BOLD_RED "\033[1;31m"
# define BOLD_GREEN "\033[1;32m"
# define BOLD_YELLOW "\033[1;33m"
# define BOLD_BLUE "\033[1;34m"
# define BOLD_MAGENTA "\033[1;35m"
# define BOLD_CYAN "\033[1;36m"
# define BOLD_WHITE "\033[1;37m"

typedef int	t_bool;

# define TRUE 1
# define FALSE 0

typedef enum e_opcode
{
	LOCK,
	UNLOCK,
	INIT,
	DESTROY,
	CREATE,
	JOIN,
	DETACH
}	t_opcode;

typedef struct s_request
{
	int		coder_idx;
	long	priority;
	long	ticket;
	long	tie;
}	t_req;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	int				dongle_id;
	long			available_at;
	pthread_cond_t	condition;
	t_req			queue[2];
	int				queue_size;
	long			seq_counter;
	t_bool			occupied;
}	t_dongle;

typedef struct s_coder
{
	int					id;
	long				compile_counter;
	long				last_compile_start;
	t_bool				finished;
	pthread_t			thread_id;
	t_dongle			*left_dongle;
	t_dongle			*right_dongle;
	struct s_reunion	*reunion;
}	t_coder;

typedef struct s_mutexes
{
	pthread_mutex_t	write;
	pthread_mutex_t	state;
}	t_mutexes;

typedef struct s_reunion
{
	long		number_of_coders;
	long		time_to_burnout;
	long		time_to_compile;
	long		time_to_debug;
	long		time_to_refactor;
	long		number_of_compiles_required;
	long		dongle_cooldown;
	long		start_simulation;
	char		*scheduler;
	t_bool		end_simulation;
	t_bool		all_threads_ready;
	t_coder		*coders;
	t_dongle	*dongles;
	t_mutexes	mutexes;
	pthread_t	monitor_thread;
}	t_reunion;

/* utils.c */
void	error_exit(const char *error);
void	error_field(const char *field, const char *error);

/* parsing/parsing_numbers.c */
long	parse_long_arg(const char *str, const char *field_name);

/* parsing/parsing_scheduler.c */
void	parse_scheduler(t_reunion *reunion, char *arg);

/* parsing/parsing.c */
void	parse_input(t_reunion *reunion, char **av);

/* init.c */
int		init_reunion(t_reunion *reunion);

/* time.c */
long	get_time_ms(void);
long	elapsed_ms(long start);
void	precise_sleep(t_reunion *reunion, long duration);

/* safe_functions.c */
void	safe_mutex_handle(pthread_mutex_t *mutex, t_opcode opcode);
void	safe_thread_handle(pthread_t *thread,
			void *(*foo)(void *),
			void *data,
			t_opcode opcode);

/* getters_setters.c */
void	set_bool(pthread_mutex_t *mutex, t_bool *dest, t_bool value);
t_bool	get_bool(pthread_mutex_t *mutex, t_bool *value);
void	set_long(pthread_mutex_t *mutex, long *dest, long value);
long	get_long(pthread_mutex_t *mutex, long *value);
void	increment_long(pthread_mutex_t *mutex, long *value);

/* coders.c */
void	grab_dongles(t_coder *coder);
void	compile(t_coder *coder);
void	debug(t_coder *coder);
void	refactor(t_coder *coder);
void	drop_dongles(t_coder *coder);

/* sim.c */
void	start_simulation(t_reunion *reunion);
void	*code_simulation(void *data);
void	*monitor_simulation(void *data);
t_bool	simulation_has_ended(t_reunion *reunion);
void	stop_simulation(t_reunion *reunion);

/* heap.c */
void	heap_push(t_req *heap, int *size, t_req elem);
t_req	heap_peek(t_req *heap, int size);
t_req	heap_pop(t_req *heap, int *size);

/* dongles.c */
void	request_dongle(t_dongle *d, t_coder *coder);
void	release_dongle(t_dongle *d, long cooldown);

/* synchro_utils.c */
void	print_state(t_coder *coder, const char *msg);
void	wait_all_threads(t_reunion *reunion);
void	cleanup_reunion(t_reunion *reunion);
void	cleanup_dongles(t_reunion *reunion, int count);

#endif
