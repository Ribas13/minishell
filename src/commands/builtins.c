/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysantos- <ysantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 19:04:24 by diosanto          #+#    #+#             */
/*   Updated: 2024/01/06 20:01:51 by ysantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

extern long long	g_exit_status;

/* Checks for invalid identifiers for a variable
"= ! @ { } -" */
bool	is_valid_id(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != '=' && !is_spaces(str[i]))
			break ;
		else if (!str[i + 1])
			return (false);
	}
	i = 0;
	if (!ft_isalpha(str[i++]))
		return (false);
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[0] != '_')
			return (false);
		i += 1;
	}
	return (true);
}

/* checks if only one arg has been provided and if so, call
the unset function */
static int	call_cmd_unset(t_statement *s, t_data *data)
{
	if (s->argc == 1)
		return (EXIT_SUCCESS);
	return (cmd_unset(s, &data->envp_lst));
}

/* If the right amount of arguments are provided, the
function cd is called otherwise the too_many_args
function is returned */
static int	call_cmd_cd(t_statement *s, t_data *data)
{
	if (s->argc > 2)
		return (cd_too_many_args());
	else
		return (cmd_cd(s->argv[1], data));
}

/* Processes the echo command and its arguments(ex: if a pipe is found
the function stops passing args to echo) and prints a new line if the
-n flag is used */
static int	call_cmd_echo(t_statement *s)
{
	t_statement	*temp;
	bool		has_n;

	temp = s;
	has_n = false;
	if (s->argc >= 2)
		has_n = streq(s->argv[1], "-n");
	cmd_echo(temp, has_n);
	temp = temp->next;
	if (!has_n)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (EXIT_SUCCESS);
}

/* Handles the execution of the builin commands */
bool	builtin(t_statement *s, t_data *data)
{
	if (streq(s->argv[0], "exit"))
		cmd_exit(s, data);
	else if (streq(s->argv[0], "unset"))
		g_exit_status = call_cmd_unset(s, data);
	else if (streq(s->argv[0], "export"))
		g_exit_status = cmd_export(s, data);
	else if (streq(s->argv[0], "cd"))
		g_exit_status = call_cmd_cd(s, data);
	else if (ft_strchr(s->argv[0], '=') && is_valid_id(s->argv[0]))
		g_exit_status = save_user_vars(s->argv[0],
				&data->envp_lst, false);
	else if (streq(s->argv[0], "echo"))
		g_exit_status = call_cmd_echo(s);
	else if (streq(s->argv[0], "pwd"))
		g_exit_status = cmd_pwd();
	else if (streq(s->argv[0], "env"))
		g_exit_status = cmd_env(data);
	else
		return (false);
	close_all_fds(NULL);
	return (true);
}
