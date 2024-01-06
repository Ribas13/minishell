/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diosanto <diosanto@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 14:37:07 by diosanto          #+#    #+#             */
/*   Updated: 2023/12/20 14:45:01 by diosanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/* Print invalid identifier error msg */
static void	invalid_id(char *var_name)
{
	ft_putstr_fd("minishell: unset: '", STDERR_FILENO);
	ft_putstr_fd(var_name, STDERR_FILENO);
	ft_putstr_fd("\': not a valid identifier", STDERR_FILENO);
}

/** @brief Removes the specified variable from the env list
 * 
 * @param var_name Variable to remove
 * @param head Pointer to the head of the envp list
 */
int	unset_var(char *var_name, t_vlst **head)
{
	t_vlst	*temp;
	t_vlst	*next;

	temp = *head;
	if (ft_strchr(var_name, '='))
	{
		invalid_id(var_name);
		return (EXIT_FAILURE);
	}
	while (temp && temp->next != NULL)
	{
		if (streq(var_name, temp->next->var_name))
		{
			next = temp->next->next;
			free(temp->next->var_name);
			free(temp->next->var_value);
			free(temp->next);
			temp->next = next;
			break ;
		}
		temp = temp->next;
	}
	return (EXIT_SUCCESS);
}

/** @brief Processes the user statement, removing one or more variables
 * from the envp list
 * 
 * @param s User statement
 * @param head Pointer to the head of the envp list
 */
int	cmd_unset(t_statement *s, t_vlst **head)
{
	int	i;

	i = 1;
	while (i < s->argc)
	{
		unset_var(s->argv[i], head);
		i++;
	}
	return (EXIT_SUCCESS);
}
