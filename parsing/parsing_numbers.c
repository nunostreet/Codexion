/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_numbers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nstreet- <nstreet-@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 17:23:00 by nstreet-          #+#    #+#             */
/*   Updated: 2026/04/09 17:45:00 by nstreet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/* Returns TRUE if c is an ASCII digit. */
static t_bool	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

/* Returns TRUE if c is a whitespace character. */
static t_bool	ft_isspace(int c)
{
	return ((c >= 9 && c <= 13) || c == ' ');
}

/* Validates format and returns pointer to the digit sequence, or exits. */
static const char	*valid_input(const char *str, const char *field_name)
{
	int			len;
	const char	*number;

	len = 0;
	while (ft_isspace(*str))
		++str;
	if (*str == '+')
		++str;
	else if (*str == '-')
		error_field(field_name, "must be a positive integer");
	if (!ft_isdigit(*str))
		error_field(field_name, "must contain only digits");
	number = str;
	while (ft_isdigit(*str))
	{
		++len;
		++str;
	}
	if (*str != '\0')
		error_field(field_name, "must contain only digits");
	if (len > 10)
		error_field(field_name, "exceeds INT_MAX");
	return (number);
}

/* Parses a non-negative integer string into a long, or exits on error. */
long	parse_long_arg(const char *str, const char *field_name)
{
	long	num;

	num = 0;
	str = valid_input(str, field_name);
	while (ft_isdigit(*str))
	{
		num = num * 10 + (*str - '0');
		++str;
	}
	if (num > INT_MAX)
		error_field(field_name, "exceeds INT_MAX");
	return (num);
}
