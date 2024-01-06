/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysantos- <ysantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 15:42:55 by diosanto          #+#    #+#             */
/*   Updated: 2024/01/06 21:40:39 by ysantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

extern long long	g_exit_status;

/* Handles the output redirection from the left to the right side
and executes the cmd

1. Close stdout
2. Duplicate write end of the pipe to STDOUT
3. Close the original read end of the pipe
4. Close the original write end of the pipe
5. Execute the cmd */
static void	left_side(t_statement *nd, t_data *data, int pipe_fds[2])
{
	if (nd->has_out_rdr == false)
	{
		close(STDOUT_FILENO);
		dup(pipe_fds[1]);
	}
	close_all_fds(pipe_fds);
	if (nd->operator == PIPE)
		nd->operator = NONE;
	exec_cmd(nd, data);
}

/* Handles the input redirection from the left to the right side
and executes the cmd

1. Close stdin
2. Duplicate read end of the pipe to stdin
3. Close the original read end of the pipe
4. Close the original write end of the pipe
5. Execute the command */
static void	right_side(t_statement *nd, t_data *data, int pipe_fds[2])
{
	if (nd->has_in_rdr == false)
	{
		close(STDIN_FILENO);
		dup(pipe_fds[0]);
	}
	close(pipe_fds[0]);
	close(pipe_fds[1]);
	if (nd->previous_has_out_rdr == true)
		dup2(ft_data()->default_stdout, STDOUT_FILENO);
	if (nd->operator == PIPE)
		exec_cmd(nd->next, data);
	else
		exec_cmd(nd, data);
}

static void	choose_node(t_statement *node, bool left_side_out_rdr
	, t_data *data, int pipes[2])
{
	if (node->operator == NONE && node->has_in_rdr == false
		&& node->has_out_rdr == false)
	{
		if (left_side_out_rdr == true)
			node->next->previous_has_out_rdr = true;
		right_side(node->next, data, pipes);
	}
	else
	{
		while (node->operator != PIPE)
			node = node->next;
		if (left_side_out_rdr == true)
			node->previous_has_out_rdr = true;
		right_side(node, data, pipes);
	}
}

/* Handles the execution of commands connected by a pipe.

1. Creates a new pipe
2. Forks a child process
3. Executes the left and right side of the pipe
4. Waits for the completion of the child process
5. Updates the global exit status */
void	exec_pipe(t_statement *node, t_data *data)
{
	pid_t	child_pid;
	int		pipe_fds[2];
	int		temp_status;
	bool	left_side_out_rdr;

	if (pipe(pipe_fds) == -1)
		panic(data, PIPE_ERR, EXIT_FAILURE);
	child_pid = fork();
	if (child_pid == -1)
		panic(data, FORK_ERR, EXIT_FAILURE);
	left_side_out_rdr = node->has_out_rdr;
	if (child_pid == 0)
		left_side(node, data, pipe_fds);
	else
	{
		choose_node(node, left_side_out_rdr, data, pipe_fds);
		waitpid(child_pid, &temp_status, 0);
	}
}
