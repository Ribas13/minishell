/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diosanto <diosanto@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 18:32:32 by diosanto          #+#    #+#             */
/*   Updated: 2024/01/02 21:37:02 by diosanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

bool	is_onid(char *str, char c)
{
	size_t	i;

	i = 0;
	while (str[i] && str[i] != '=')
	{
		if (str[i] == c)
			return (true);
		i++;
	}
	return (false);
}

/* Print the export statement */
static int	single_export(t_data *data)
{
	t_vlst	*temp;

	temp = data->envp_lst;
	while (temp != NULL)
	{
		if (temp->is_exported && temp->var_value)
			printf("declare -x %s=\"%s\"\n", temp->var_name, temp->var_value);
		else if (temp->is_exported && !temp->var_value)
			printf("declare -x %s\n", temp->var_name);
		temp = temp->next;
	}
	return (EXIT_SUCCESS);
}

/* Prints "bad identifier" message */
static int	export_bad_id(char *id)
{
	ft_putstr_fd("minishell: export: '", STDERR_FILENO);
	ft_putstr_fd(id, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
	return (EXIT_FAILURE);
}

/* Loop through the envp list and set the variables to
"exported" when the target var is found */
static bool	loop_and_exp(char *var_name, t_data *data)
{
	t_vlst	*temp;

	temp = data->envp_lst;
	while (temp != NULL)
	{
		if (streq(var_name, temp->var_name))
		{
			temp->is_exported = true;
			return (true);
		}
		temp = temp->next;
	}
	return (false);
}

/* Implementation of export command */
int	cmd_export(t_statement *statement, t_data *data)
{
	int		exit_status;
	size_t	i;

	if (statement->argc == 1)
		return (single_export(data));
	exit_status = EXIT_SUCCESS;
	i = 0;
	while (statement->argv[++i])
	{
		if (!is_valid_id(statement->argv[i]))
			exit_status = export_bad_id(statement->argv[i]);
		else if (!ft_strchr(statement->argv[i], '=')
			&& loop_and_exp(statement->argv[i], data))
			continue ;
		else if (statement->argv[1])
			save_user_vars(statement->argv[i], &data->envp_lst, true);
	}
	return (exit_status);
}
