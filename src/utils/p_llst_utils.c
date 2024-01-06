/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_llst_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diosanto <diosanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 18:49:05 by diosanto          #+#    #+#             */
/*   Updated: 2024/01/06 18:49:28 by diosanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Will delete the next node of the list, giving the current node
the new operator and next values. */
void	p_lst_delone(t_statement *node)
{
	t_statement	*to_clean;

	to_clean = node->next;
	node->next = to_clean->next;
	node->operator = to_clean->operator;
	free_matrix(to_clean->argv);
	free(to_clean);
}

void	p_lstclear(t_statement **head)
{
	t_statement	*temp;
	t_statement	*next_node;

	if (!head)
		return ;
	temp = *head;
	while (temp != NULL)
	{
		next_node = temp->next;
		free_matrix(temp->argv);
		free(temp);
		temp = next_node;
	}
	*head = NULL;
}

size_t	p_lstsize(t_statement *head)
{
	t_statement	*temp;
	size_t		size;

	temp = head;
	size = 0;
	while (temp != NULL)
	{
		temp = temp->next;
		size += 1;
	}
	return (size);
}

t_statement	*p_new_node(int ac)
{
	t_statement	*new_node;

	new_node = malloc(sizeof(t_statement));
	new_node->argc = ac;
	new_node->argv = ft_calloc((ac + 1), sizeof(char *));
	new_node->operator = NONE;
	new_node->next = NULL;
	new_node->has_out_rdr = false;
	new_node->has_in_rdr = false;
	new_node->previous_has_out_rdr = false;
	new_node->has_error = false;
	return (new_node);
}
