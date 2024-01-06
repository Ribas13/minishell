/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diosanto <diosanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 16:28:09 by diosanto          #+#    #+#             */
/*   Updated: 2024/01/06 20:19:09 by diosanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
}
