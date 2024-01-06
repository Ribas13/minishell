/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v_llst_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysantos- <ysantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 16:22:15 by diosanto          #+#    #+#             */
/*   Updated: 2023/12/25 23:40:01 by ysantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Returns the amount of elements that are exported in the list(head) */
size_t	v_lstsize_exported(t_vlst **head)
{
	t_vlst	*temp;
	size_t	count;

	temp = *head;
	count = 0;
	while (temp != NULL)
	{
		if (temp->is_exported)
			count += 1;
		temp = temp->next;
	}
	return (count);
}

/* Returns the exported state of a given node(variable) */
bool	get_exported_state(char *var_name, t_vlst **head)
{
	t_vlst	*temp;

	temp = *head;
	while (temp != NULL)
	{
		if (streq(var_name, temp->var_name) && temp->var_value)
			return (temp->is_exported);
		temp = temp->next;
	}
	return (false);
}

/* Splits an environment variable string into an array of
strings where the fist element if the vars name, the second
is the var value and the third is NULL */
char	**split_envp(char *env)
{
	char	**splited;
	size_t	i_of_equal;

	splited = ft_calloc(3, sizeof(char *));
	i_of_equal = ft_strchr(env, '=') - env;
	if (is_onstr(env, '='))
	{
		splited[0] = ft_substr(env, 0, i_of_equal);
		splited[1] = ft_substr(env, i_of_equal + 1,
				ft_strlen(&env[i_of_equal]));
	}
	else
		splited[0] = ft_strdup(env);
	return (splited);
}

/* Updates the export state of a variable, unsets the variable
in the list and adds a new node to the list with the updated var info

1. Split the statement into an array(line)
2. Check if the variable is already marked for export
3. Unset the variable in the first chunck of the statement
4. Add the new variable to the envp list
6. Free the line that had the statement */
int	save_user_vars(char *statement, t_vlst **head, bool to_export)
{
	char	**line;

	line = split_envp(statement);
	if (get_exported_state(line[0], head) && !to_export)
		to_export = true;
	unset_var(line[0], head);
	v_lstadd_back(head, v_new_node(line[0], line[1], to_export));
	free(line);
	return (EXIT_SUCCESS);
}
