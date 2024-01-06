/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diosanto <diosanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 17:02:58 by diosanto          #+#    #+#             */
/*   Updated: 2024/01/06 18:48:25 by diosanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

long long	g_exit_status = 0;

void	panic(t_data *data, char *error_msg, int exit_status)
{
	if (data)
		destroy(data);
	ft_putendl_fd(error_msg, STDERR_FILENO);
	exit(exit_status);
}

static char	*get_input(void)
{
	char	*raw_input;
	char	*input;
	bool	save;

	raw_input = readline("\e[0;94mminishell$\e[0m ");
	save = false;
	if (raw_input && !is_spaces(raw_input[0]) && ft_strlen(raw_input))
		save = true;
	input = trim_free(raw_input, " \t");
	if (save)
		add_history(input);
	return (input);
}

/*Clean up the parsed list and reset data structure

1. Clear the parsed statement list
2. Set the pointer to the statement list to NULL
3. Set the head of the data structure to NULL*/
static void	clean_parsed(t_statement **statement_list, t_data *data)
{
	p_lstclear(statement_list);
	*statement_list = NULL;
	data->head = NULL;
}

void	set_def_std(void)
{
	ft_data()->default_stdin = dup(STDIN_FILENO);
	ft_data()->default_stdout = dup(STDOUT_FILENO);
}

/*
1. check for args being provided upon execution(error)
2. start the shell environment
3. get the user input
4. check if valid
5. add the input to the history
6. expand the input and add special characters in the input
7. check if the modified input is empty, if so, free
8. parse the input into a list of statements(statement_list var)
9. set the head of the data structure to the parsed statement list
10. exec the parsed statements
11. clean the list and reset the data structure
12. exit with sucess(not sure if it will ever reach this)*/
int	main(int ac, char **av, char **envp)
{
	t_data		data;
	t_statement	*statement_list;
	char		*input;

	if (av && ac > 1)
		panic(NULL, CL_ARGUMENTS_ERR, EXIT_FAILURE);
	setup_shell(envp, &data, &statement_list);
	set_def_std();
	while (1)
	{
		input = get_input();
		if (!valid_input(input, &data))
			continue ;
		input = expander(input, &data);
		if (!input[0])
		{
			free(input);
			continue ;
		}
		statement_list = parser(input);
		data.head = statement_list;
		exec_type(statement_list, &data);
		clean_parsed(&statement_list, &data);
	}
	return (EXIT_SUCCESS);
}
