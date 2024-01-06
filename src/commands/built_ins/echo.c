/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diosanto <diosanto@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 17:36:50 by diosanto          #+#    #+#             */
/*   Updated: 2023/12/15 17:46:15 by diosanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/* Prints the string(but skips backslashes '\\')
in order to mimic the behaviour of the echo cmd */
static void	print(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\\')
		{
			i++;
			continue ;
		}
		ft_putchar_fd(str[i], STDOUT_FILENO);
		i++;
	}
}

/* Prints the arguments provided for the echo cmd.
formats the output with spaces betwee the args.
Also handles the -n flag */
int	cmd_echo(t_statement *statement, bool has_n)
{
	int	i;

	i = 1;
	if (has_n)
		i += 1;
	while (statement->argv[i] && i != statement->argc)
	{
		print(statement->argv[i]);
		if (!statement->argv[i + 1] && statement->operator != RDR_INPUT)
			break ;
		ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	return (EXIT_SUCCESS);
}
