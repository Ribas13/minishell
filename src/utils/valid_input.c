/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysantos- <ysantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 17:37:22 by diosanto          #+#    #+#             */
/*   Updated: 2023/12/26 01:03:50 by ysantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

extern long long	g_exit_status;

/* Prints an "exit" message, sets the global exit status and
cleans data structure if it's not empty, and exits the shell with the
provided exit status */
static void	exit_shell(int exit_status, t_data *data)
{
	ft_putendl_fd("exit", STDOUT_FILENO);
	g_exit_status = exit_status;
	if (data)
		destroy(data);
	exit(g_exit_status);
}

static void	valid_input_exit(char *input, t_data *data)
{
	if (input == NULL)
	{
		free(input);
		exit_shell(EXIT_SUCCESS, data);
	}
	else
		return ;
}

/* Checks the validity of the user's input. Considers cases such as NULL
input, empty input, unclosed quotes and syntax related issues.
Prints error messages if needed, sets global exit status and frees
memory allocated for the input. */
bool	valid_input(char *input, t_data *data)
{
	bool	valid;

	valid = true;
	valid_input_exit(input, data);
	if (input[0] == '\0' || !ft_strlen(input))
	{
		free (input);
		return (false);
	}
	else if (unclosed_quotes(input))
	{
		ft_putendl_fd(UNCLOSED_QUOTES, STDERR_FILENO);
		valid = false;
	}
	else if (invalid_syntax(input) || invalid_syntax2(input)
		|| invalid_syntax_on_operator(input))
		valid = false;
	if (!valid)
	{
		free(input);
		g_exit_status = 2;
	}
	return (valid);
}
