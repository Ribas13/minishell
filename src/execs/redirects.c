/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diosanto <diosanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 16:28:09 by diosanto          #+#    #+#             */
/*   Updated: 2024/01/06 18:19:23 by diosanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

extern long long	g_exit_status;

void	print_lst(t_statement *node)
{
	t_statement *temp;

	temp = node;
	int i = 0;
	while (temp->next != NULL)
	{
		i = 0;
		//while (temp->argv[i])
		//{
			printf("\n%s has an operator of ", temp->argv[i]);

			if (temp->operator == RDR_INPUT)
				printf("input redirection\n");
			else if (temp->operator == RDR_INPUT_UNTIL)
				printf("HEREDOC\n");
			else if (temp->operator == RDR_OUT_APPEND || temp->operator == RDR_OUT_REPLACE)
				printf("ouput redirection\n");
			else if (temp->operator == PIPE)
				printf("pipe\n");
			else
				printf("nothing\n");
			while (temp->argv[++i])
				printf("\tARGS: %s\n", temp->argv[i]);
		//}
		temp = temp->next;
	}
	i = 0;
	printf("\n%s has an operator of ", temp->argv[0]);
			if (temp->operator == RDR_INPUT)
				printf("input redirection\n");
			else if (temp->operator == RDR_INPUT_UNTIL)
				printf("HEREDOC\n");
			else if (temp->operator == RDR_OUT_APPEND || temp->operator == RDR_OUT_REPLACE)
				printf("ouput redirection\n");
			else if (temp->operator == PIPE)
				printf("pipe\n");
			else
				printf("nothing\n");
			while (temp->argv[++i])
				printf("\tARGS: %s\n", temp->argv[i]);
} 

static void	redirect_input_until(t_statement *node)
{
	char		*buff;
	int			fd[2];
	t_statement	*temp;

	dup2(ft_data()->default_stdin, STDIN_FILENO);
	pipe(fd);
	buff = NULL;
	temp = node;
	signal(SIGQUIT, &dismiss_signal);
	while (node->next != NULL && node->operator != PIPE)
	{
		if (node->operator == RDR_INPUT_UNTIL)
		{
			while (1)
			{
				buff = readline("> ");
				if (buff == NULL || streq(buff, node->next->argv[0]))
				{
					if (buff != NULL && !is_last_delim(node, node->next->argv[0]))
						free(buff);
					break ;
				}
				if (is_last_delim(node, node->argv[0]))
					ft_putendl_fd(buff, fd[1]);
				free(buff);
			}
			node = node->next;
		}
		else if (node->operator == RDR_INPUT || node->operator == RDR_OUT_APPEND
			|| node->operator == RDR_OUT_REPLACE || node->operator == NONE)
			node = node->next;
	}
	signal(SIGQUIT, SIG_IGN);
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	check_files(temp);
}

static void	redirect_input(t_statement *node, t_statement *head)
{
	int		in_file;
	char	*error_msg_prefix;

	if (access(node->argv[0], F_OK) == 0)
	{
		in_file = open(node->argv[0], O_RDONLY, 0666);
		dup2(in_file, STDIN_FILENO);
	}
	else if (head->has_error == false)
	{
		error_msg_prefix = ft_strjoin("minishell: ", node->argv[0]);
		perror(error_msg_prefix);
		free(error_msg_prefix);
		g_exit_status = 1;
		head->has_error = true;
	}
}

//check what the last inpt redir is 
//(1 for heredoc, -1 for infile, 0 for none)
// if -1 --> jump to last_delim and run heredoc, when done, run infile
// else if 1 --> jump to last_delim and run heredoc
//exit
void	set_input(t_statement *node)
{
	t_statement	*temp;
	t_statement	*last_infile;

	temp = node;
	if (has_input_rdr(node) == -1)
	{
		last_infile = get_last_infile(node);
		if (has_heredoc(node))
		{
			while (temp->next != NULL)
			{
				temp->has_in_rdr = true;
				if (temp->operator == RDR_INPUT_UNTIL
					&& is_last_delim(node, temp->argv[0]))
					fake_heredoc(node);
				temp = temp->next;
			}
			temp->has_in_rdr = true;
			redirect_input(last_infile, node);
		}
		else
		{
			//check_files(temp);
			redirect_input(last_infile, node);
		}
	}
	else if (has_input_rdr(node) == 1)
		redirect_input_until(node);
}

t_statement	*jump_to_endcmd(t_statement *node)
{
	while (node->next != NULL)
	{
		if (node->operator == PIPE || node->operator == NONE)
			return (node);
		node = node->next;
	}
	return (node);
}

bool	is_last_out_rdr(t_statement *node, t_statement *nd_to_check)
{
	t_statement	*temp;
	char		*last_rdr;

	temp = node;
	while (temp->next != NULL)
	{
		if (temp->operator == PIPE)
			break ;
		if (temp->operator == RDR_OUT_APPEND || temp->operator == RDR_OUT_REPLACE)
			last_rdr = temp->next->argv[0];
		temp = temp->next;
	}
	if (temp->operator == RDR_OUT_APPEND || temp->operator == RDR_OUT_REPLACE)
		last_rdr = temp->next->argv[0];
	if (streq(nd_to_check->argv[0], last_rdr))
		return (true);
	else
		return (false);
}

void	out_error(t_statement *node, t_statement *head)
{
	char	*error_msg_prefix;

	if (head->has_error == false && access(node->next->argv[0], F_OK | W_OK) != 0)
	{
		error_msg_prefix = ft_strjoin("minishell: ", node->next->argv[0]);
		perror(error_msg_prefix);
		free(error_msg_prefix);
		g_exit_status = 1;
		head->has_error = true;
		//exit(EXIT_FAILURE);
	}
	/* else if (access(node->next->argv[0], W_OK) != 0)
	{
		error_msg_prefix = ft_strjoin("minishell: ", "");
		perror(error_msg_prefix);
		free(error_msg_prefix);
		g_exit_status = 1;
		exit(EXIT_FAILURE);
	} */
}

void	set_output(t_statement *node)
{
	t_statement	*temp;
	//char		*last_output_redir;

	temp = node;
	if (has_output_redir(node) == true)
	{
		temp->has_out_rdr = true;
		close(STDOUT_FILENO);
		while (temp->next != NULL)
		{
			temp->has_out_rdr = true;
			if (temp->operator == PIPE)
				break ;
			if (temp->operator == RDR_OUT_REPLACE)
			{
				if (node->has_error == false && open(temp->next->argv[0], O_WRONLY | O_TRUNC | O_CREAT, 0666) == -1)
					out_error(temp, node);
			}
			else if (temp->operator == RDR_OUT_APPEND)
			{
				if (node->has_error == false && open(temp->next->argv[0], O_WRONLY | O_APPEND | O_CREAT, 0666) == -1)
					out_error(temp, node);
			}
			temp = temp->next;
			if (!is_last_out_rdr(node, temp))
				close(1);
		}
		temp->has_out_rdr = true;
		/* if (temp->operator == RDR_OUT_REPLACE)
		{
			if (open(temp->next->argv[0], O_WRONLY | O_TRUNC | O_CREAT, 0666) == -1)
				out_error(temp);
		}
		else if (temp->operator == RDR_OUT_APPEND)
		{
			if (open(temp->next->argv[0], O_WRONLY | O_APPEND | O_CREAT, 0666) == -1)
				out_error(temp);
		} */
	}
}

/* Handles redirections
Redirects the output based on the type of operator,
resets the operator and continues to execute the command
or handle pipes */
void	exec_redirects(t_statement *node, t_data *data)
{
	t_statement	*temp;

	temp = node;
	check_files(node);
	set_input(node);
	set_output(node);
	node = jump_to_endcmd(node);
	temp->operator = NONE;
	if (node->operator == NONE)
	{
		exec_cmd(temp, data);
	}
	else
		exec_pipe(temp, data);
	//exec_pipe(temp, data);
}
