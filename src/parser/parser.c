/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysantos- <ysantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/22 16:31:35 by diosanto          #+#    #+#             */
/*   Updated: 2024/01/03 23:31:25 by ysantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

extern long long	g_exit_status;

char	**parse_input(char *input)
{
	char		**parsed;
	size_t		i;
	size_t		k;
	size_t		len;
	size_t		j;

	i = 0;
	k = 0;
	parsed = malloc((get_nr_statements(input) + 1) * sizeof(char *));
	while (input[i])
	{
		len = get_token_len(&input[i]);
		if (!len)
		{
			i += 1;
			continue ;
		}
		parsed[k] = malloc((len + 1) * sizeof(char));
		j = 0;
		while (input[i] && j < len)
			parsed[k][j++] = input[i++];
		parsed[k++][j] = '\0';
	}
	parsed[k] = NULL;
	return (parsed);
}

t_statement	*parser(char *input)
{
	char		**parsed;
	t_statement	*temp;
	t_statement	*head;
	size_t		i;
	size_t		j;

	parsed = parse_input(input);
	free(input);
	temp = p_new_node(get_ac(&parsed[0]));
	head = temp;
	i = 0;
	while (parsed[i])
	{
		j = 0;
		while (parsed[i] && !is_onstr(OPERATORS, parsed[i][0]))
			temp->argv[j++] = remove_quotes(parsed[i++]);
		if (!parsed[i])
			break ;
		temp->operator = get_operator(parsed[i++]);
		temp->next = p_new_node(get_ac(&parsed[j]));
		temp = temp->next;
	}
	free(parsed);
	join_args(head);
	return (head);
}
