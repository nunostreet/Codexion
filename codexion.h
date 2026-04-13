/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nunostreet <nunostreet@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 16:21:43 by nstreet-          #+#    #+#             */
/*   Updated: 2026/04/13 12:43:40 by nunostreet       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
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

typedef pthread_mutex_t	t_mtx;

typedef struct s_dongle
{
	t_mtx	mutex;
	int		dongle_id;
	bool	available;
	long	available_at;
}	t_dongle;

typedef struct s_coder
{
	int					id;
	long				compile_counter;
	long				last_compile_start;
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
	bool		end_simulation;
	t_coder		*coders;
	t_dongle	*dongles;
	t_mtx		write_mutex;
	t_mtx		state_mutex;
	pthread_t	monitor_thread;
}	t_reunion;

void	error_exit(const char *error);
void	error_field(const char *field, const char *error);
int		init_reunion(t_reunion *reunion);
long	parse_long_arg(const char *str, const char *field_name);
void	parse_scheduler(t_reunion *reunion, char *arg);
void	parse_input(t_reunion *reunion, char **av);

#endif
