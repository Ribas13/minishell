/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysantos- <ysantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 14:45:59 by diosanto          #+#    #+#             */
/*   Updated: 2024/01/06 19:54:33 by ysantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

extern long long	g_exit_status;

/** @brief Executes a single command
 * @param curr_node Command to execute
 * @param data Program data
 */
void	exec_cmd(t_statement *curr_node, t_data *data)
{
	signal(SIGINT, SIG_DFL);
	if (curr_node->operator == PIPE)
		exec_pipe(curr_node, data);
	else if (curr_node->operator == NONE)
	{
		if (curr_node->has_error == true)
			exit(EXIT_FAILURE);
		exec_executables(curr_node, data);
	}
	else
		exec_redirects(curr_node, data);
	exit(g_exit_status);
}

/* Handles the signal that made child terminate. */
void	child_signal(int temp_status)
{
	if (WTERMSIG(temp_status) == SIGQUIT)
	{
		g_exit_status = 131;
		printf("Quit (core dumped)\n");
	}
	else if (WTERMSIG(temp_status) == SIGINT)
	{
		g_exit_status = 130;
	}
}

/* void	print_statements(t_statement *node)
{
	int n = 0;
	while (node)	{
		printf("node %d\n", n++);
		for (int i = 0; node->argv[i]; i++)	{
			printf("node->argv[%d] = \"%s\", ", i, node->argv[i]);
		}
		printf("\n");
		node = node->next;
	}
	printf("\n\n");
} */

/* Determines if the execution will take place in the current process
or if there's a need to create a separate one for the execution(ie. fork)
if there's more than one command to be run

1. if only one command, check if it's builtin or external and run
2. else, fork and exec the commands in a separate process
3. wait for the child process to end (also saves the exit status to the
temp_status var)
4. WTERMSIG extracts the signal number that caused the process to end
5. bit shift the temp_status to save the exit_status(the exit status
is stored in the last 8 bits of the var, thus isolating the exit status) */
void	exec_type(t_statement *statement_list, t_data *data)
{
	int	temp_status;

	signal(SIGINT, SIG_IGN);
	temp_status = 0;
	if (p_lstsize(statement_list) == 1)
	{
		if (!builtin(statement_list, data))
		{
			if (fork() == 0)
			{
				signal(SIGINT, SIG_DFL);
				signal(SIGQUIT, SIG_DFL);
				close_all_fds(NULL);
				exec_executables(statement_list, data);
			}
		}
		else
		{
			signal(SIGINT, &dismiss_signal);
			return ;
		}
	}
	else if (fork() == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		exec_cmd(statement_list, data);
		close_all_fds(NULL);
	}
	waitpid(-1, &temp_status, 0);
	signal(SIGINT, &dismiss_signal);
	if (WTERMSIG(temp_status))
		child_signal(temp_status);
	else
		g_exit_status = WEXITSTATUS(temp_status);
}
