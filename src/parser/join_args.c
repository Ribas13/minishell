/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diosanto <diosanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 18:50:12 by diosanto          #+#    #+#             */
/*   Updated: 2024/01/06 18:51:02 by diosanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Return copy of array with a copy of string added to it.
	Array is already freed inside. */
static char	**add_str_to_array(char **array, char *str)
{
	int		i;
	char	**new_array;

	i = 0;
	while (array[i])
		i++;
	new_array = ft_calloc(sizeof(char *), (i + 2));
	i = 0;
	while (array[i])
	{
		new_array[i] = array[i];
		i++;
	}
	new_array[i] = ft_strdup(str);
	new_array[i + 1] = NULL;
	free(array);
	return (new_array);
}

/* Remove the array of index i and jump everyone 1 futher. */
static void	jump_one_array(char **array, int i)
{
	free(array[i]);
	array[i] = array[i + 1];
	while (array[++i])
		array[i] = array[i + 1];
}

/** @brief Will move all extra arguments from one node to another
 * @param cmd Node that contains the command
 * @param move Node that contains the arguments */
static void	move_extra_args(t_statement *cmd, t_statement *move)
{
	int	i;

	i = 1;
	while (move->argv[i])
	{
		cmd->argv = add_str_to_array(cmd->argv, move->argv[i]);
		jump_one_array(move->argv, i);
		cmd->argc++;
	}
}

/* For every command it will take nodes without INPUT/OUTPUT
 * and pass their args as an argument for the first node */
void	join_args(t_statement *head)
{
	t_statement	*cmd;
	t_statement	*tmp;

	cmd = head;
	while (cmd)
	{
		tmp = cmd;
		while (tmp->next && tmp->operator != PIPE)
		{
			move_extra_args(cmd, tmp->next);
			tmp = tmp->next;
		}
		cmd = tmp->next;
	}
}
