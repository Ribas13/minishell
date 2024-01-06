/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diosanto <diosanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 18:49:21 by diosanto          #+#    #+#             */
/*   Updated: 2024/01/06 20:28:54 by diosanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*get_last_delim(t_statement *node)
{
	t_statement	*temp;
	char		*last_delim;

	temp = node;
	last_delim = NULL;
	while (temp->next != NULL && temp->next->operator != PIPE)
	{
		if (temp->operator == RDR_INPUT_UNTIL)
			last_delim = temp->argv[0];
		temp = temp->next;
	}
	if (temp->operator == RDR_INPUT_UNTIL)
		last_delim = temp->argv[0];
	return (last_delim);
}

bool	is_last_delim(t_statement *node, char *delim)
{
	char	*last_delim;

	last_delim = get_last_delim(node);
	if (streq(delim, last_delim))
		return (true);
	else
		return (false);
}

int	has_input_rdr(t_statement *node)
{
	t_statement	*temp;
	int			res;

	temp = node;
	res = 0;
	while (temp->next != NULL)
	{
		if (temp->operator == PIPE)
			break ;
		if (temp->operator == RDR_INPUT)
			res = -1;
		else if (temp->operator == RDR_INPUT_UNTIL)
			res = 1;
		temp = temp->next;
	}
	return (res);
}

bool	has_heredoc(t_statement *node)
{
	t_statement	*temp;

	temp = node;
	while (temp->next != NULL && temp->operator != PIPE)
	{
		if (temp->operator == RDR_INPUT_UNTIL)
			return (true);
		temp = temp->next;
	}
	if (temp->operator == RDR_INPUT_UNTIL)
		return (true);
	return (false);
}

void	fake_heredoc(t_statement *node)
{
	char		*buff;

	while (node->next != NULL && node->operator != PIPE)
	{
		while (1)
		{
			buff = readline("> ");
			if (streq(buff, node->next->argv[0]))
			{
				if (!is_last_delim(node, node->next->argv[0]))
					free(buff);
				break ;
			}
			if (is_last_delim(node, node->argv[0]))
				ft_putstr_fd("", STDIN_FILENO);
		}
		if (is_last_delim(node, node->argv[0]))
			break ;
		node = node->next;
	}
}
