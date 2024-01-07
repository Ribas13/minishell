/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rdr_outputs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysantos- <ysantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 20:18:31 by diosanto          #+#    #+#             */
/*   Updated: 2024/01/06 23:46:13 by ysantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

extern long long	g_exit_status;

bool	is_last_out_rdr(t_statement *node, t_statement *nd_to_check)
{
	t_statement	*temp;
	char		*last_rdr;

	temp = node;
	while (temp->next != NULL)
	{
		if (temp->operator == PIPE)
			break ;
		if (temp->operator == RDR_OUT_APPEND
			|| temp->operator == RDR_OUT_REPLACE)
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

	if (head->has_error == false
		&& access(node->next->argv[0], F_OK | W_OK) != 0)
	{
		error_msg_prefix = ft_strjoin("minishell: ", node->next->argv[0]);
		perror(error_msg_prefix);
		free(error_msg_prefix);
		g_exit_status = 1;
		close_all_fds(NULL);
		head->has_error = true;
	}
}

int	ft_open(t_statement *temp, t_statement *node, bool append)
{
	int	fd;

	fd = -1;
	if (!append && node->has_error == false)
	{
		fd = open(temp->next->argv[0], O_WRONLY | O_TRUNC | O_CREAT, 0666);
		if (fd == -1)
			out_error(temp, node);
	}
	if (append && node->has_error == false)
	{
		fd = open(temp->next->argv[0], O_WRONLY | O_APPEND | O_CREAT, 0666);
		if (fd == -1)
			out_error(temp, node);
	}
	return (fd);
}

void	set_output(t_statement *node)
{
	t_statement	*temp;
	int			fd;

	fd = -1;
	temp = node;
	if (has_output_redir(node) == true)
	{
		temp->has_out_rdr = true;
		while (temp->next != NULL)
		{
			temp->has_out_rdr = true;
			if (temp->operator == PIPE)
				break ;
			if (temp->operator == RDR_OUT_REPLACE)
				fd = ft_open(temp, node, false);
			else if (temp->operator == RDR_OUT_APPEND)
				fd = ft_open(temp, node, true);
			temp = temp->next;
			if (fd != -1 && is_last_out_rdr(node, temp))
				dup2(fd, STDOUT_FILENO);
			if (fd != -1)
				close(fd);
		}
		temp->has_out_rdr = true;
	}
}
