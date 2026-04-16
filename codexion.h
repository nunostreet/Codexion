/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nunostreet <nunostreet@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 16:21:43 by nstreet-          #+#    #+#             */
/*   Updated: 2026/04/16 15:14:11 by nunostreet       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# define RST "\033[0m"
# define BOLD_RED "\033[1;31m"
# define BOLD_GREEN "\033[1;32m"
# define BOLD_YELLOW "\033[1;33m"
# define BOLD_BLUE "\033[1;34m"
# define BOLD_MAGENTA "\033[1;35m"
# define BOLD_CYAN "\033[1;36m"
# define BOLD_WHITE "\033[1;37m"

typedef	int	t_bool;

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
	DETACH,
}	t_opcode;

typedef pthread_mutex_t	t_mtx;

typedef struct s_dongle
{
	t_mtx	mutex;
	int		dongle_id;
	t_bool	available;
	long	available_at;
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
	t_mtx		write_mutex;
	t_mtx		state_mutex;
	pthread_t	monitor_thread;
}	t_reunion;

typedef struct s_request
{
	struct s_coder	*coder;
	long			priority;
	long			seq;
}	t_request;

/* Parsing arguments */
long	parse_long_arg(const char *str, const char *field_name);
void	parse_scheduler(t_reunion *reunion, char *arg);
void	parse_input(t_reunion *reunion, char **av);

/* Error helpers */
void	error_exit(const char *error);
void	error_field(const char *field, const char *error);

/* Initializing variables */
int		init_reunion(t_reunion *reunion);

/* Time functions */
long	get_time_ms(void);
long	elapsed_ms(long start);

/* Safe functions to handle errors */
void	safe_mutex_handle(t_mtx *mutex, t_opcode opcode);
void	safe_thread_handle(pthread_t *thread, void *(*foo)(void *), void *data,
t_opcode opcode);

/* Getters and setters to avoid repetition */
void	set_bool(t_mtx *mutex, t_bool *dest, t_bool value);
t_bool	get_bool(t_mtx *mutex, t_bool *value);
void	set_long(t_mtx *mutex, long *dest, long value);
long	get_long(t_mtx *mutex, long *value);
void	increment_long(t_mtx *mutex, long *value);

/* Simulation functions */
void	start_simulation(t_reunion *reunion);
void	*code_simulation(void *data);
void	*monitor_simulation(void *data);

void	wait_all_threads(t_reunion *reunion); // spinlock
t_bool	simulation_has_ended(t_reunion *reunion);
void	stop_simulation(t_reunion *reunion);

void	print_state(t_coder *coder, const char *msg);
void	precise_sleep(t_reunion *reunion, long duration); // needed?

t_bool	try_take_dongles(t_coder *coder);
void	release_dongles(t_coder *coder);
void	cleanup_reunion(t_reunion *reunion);

#endif
