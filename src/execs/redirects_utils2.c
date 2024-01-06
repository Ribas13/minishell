/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diosanto <diosanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 00:25:43 by diosanto          #+#    #+#             */
/*   Updated: 2024/01/06 21:44:45 by diosanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

extern long long	g_exit_status;

t_statement	*get_last_infile(t_statement *node)
{
	t_statement	*temp;
	t_statement	*last_infile;

	temp = node;
	while (temp->next != NULL)
	{
		if (temp->operator == PIPE)
			break ;
		if (temp->operator == RDR_INPUT)
			last_infile = temp->next;
		temp = temp->next;
	}
	if (temp->operator == RDR_INPUT)
		last_infile = temp->next;
	return (last_infile);
}

void	check_input(t_statement *temp, t_statement *node)
{
	char		*error_msg_prefix;

	if (node->has_error == false
		&& (temp->operator == NONE || temp->operator == RDR_INPUT)
		&& access(temp->next->argv[0], F_OK) != 0)
	{
		error_msg_prefix = ft_strjoin("minishell: ",
				temp->next->argv[0]);
		perror(error_msg_prefix);
		free(error_msg_prefix);
		g_exit_status = 2;
		node->has_error = true;
	}
}

void	check_output(t_statement *temp, t_statement *node)
{
	char		*error_msg_prefix;

	if ((temp->operator == RDR_OUT_APPEND
			|| temp->operator == RDR_OUT_REPLACE)
		&& node->has_error == false
		&& access(node->next->argv[0], F_OK) == 0
		&& access(node->next->argv[0], W_OK) == -1)
	{
		error_msg_prefix = ft_strjoin("check_output: ", temp->next->argv[0]);
		perror(error_msg_prefix);
		free(error_msg_prefix);
		g_exit_status = 1;
		node->has_error = true;
	}
}

/* Check if a file used as redirection in the cmd line exists,
throws error if the file doesn't exist and exits the execution */
void	check_files(t_statement *node)
{
	t_statement	*temp;

	temp = node;
	while (temp->next != NULL)
	{
		if (temp->operator == PIPE || temp->next->operator == PIPE)
			break ;
		temp->has_in_rdr = true;
		check_input(temp, node);
		check_output(temp, node);
		temp = temp->next;
	}
}

bool	has_output_redir(t_statement *node)
{
	t_statement	*temp;

	temp = node;
	while (temp->next != NULL)
	{
		if (temp->operator == PIPE)
			break ;
		if (temp->operator == RDR_OUT_APPEND
			|| temp->operator == RDR_OUT_REPLACE)
			return (true);
		temp = temp->next;
	}
	if (temp->operator == RDR_OUT_APPEND
		|| temp->operator == RDR_OUT_REPLACE)
		return (true);
	return (false);
}
