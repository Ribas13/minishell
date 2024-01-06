/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fits_longlong.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diosanto <diosanto@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 18:09:37 by diosanto          #+#    #+#             */
/*   Updated: 2023/12/15 18:23:19 by diosanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * Checks if a string can be converted into long long
 * (ie. if it will fit and not cause any overflows)
 * 1. if a str is bigger than 20, it will overflow
 * 2. if the string is "-9223372036854775808", it can be
 * converted, as thats the min value for long long
 * 3. once it gets to the loop, every time we check if it
 * has overflowed (if (out > (LLONG_MAX - c) / 10))
 * 4. returns true or false based on if it can fit inside a long long
  */
bool	fits_longlong(char *str)
{
	long long	out;
	int			c;

	if (ft_strlen(str) > 20)
		return (false);
	if (streq("-9223372036854775808", str))
		return (true);
	out = 0;
	if (*str == '-' || *str == '+')
		str++;
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		c = *str - '0';
		if (out > (LLONG_MAX - c) / 10)
			return (false);
		out = out * 10 + c;
		str += 1;
	}
	return (true);
}
