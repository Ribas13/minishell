/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rdr_inputs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysantos- <ysantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 20:14:48 by diosanto          #+#    #+#             */
/*   Updated: 2024/01/06 21:31:58 by ysantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

extern long long	g_exit_status;

static void	ft_heredoc(t_statement *node, int fd[2], char *buff)
{
	while (node->next != NULL && node->operator != PIPE)
	{
		if (node->operator == RDR_INPUT_UNTIL)
		{
			while (1)
			{
				buff = readline("> ");
				if (buff == NULL || streq(buff, node->next->argv[0]))
				{
					if (buff != NULL)
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
	ft_heredoc(node, fd, buff);
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
			redirect_input(last_infile, node);
	}
	else if (has_input_rdr(node) == 1)
		redirect_input_until(node);
}
