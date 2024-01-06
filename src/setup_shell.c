/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_shell.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysantos- <ysantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 17:27:34 by diosanto          #+#    #+#             */
/*   Updated: 2023/12/31 21:11:40 by ysantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*1. Set the envp member of data to the provided envp
2. Initialize the envp_lst member of data(init_envp_lst())
3. Reset the head(data)
4. Set the statement_list str to NULL to empty it
5. Call config_signals()*/
void	setup_shell(char **envp, t_data *data, t_statement **statement_list)
{
	data->envp = envp;
	data->envp_lst = init_envp_lst(envp);
	data->head = NULL;
	data->default_stdin = dup(STDIN_FILENO);
	data->default_stdout = dup(STDOUT_FILENO);
	*statement_list = NULL;
	ft_data()->parent = true;
	config_signals();
}
