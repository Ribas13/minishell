/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diosanto <diosanto@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/22 16:40:05 by diosanto          #+#    #+#             */
/*   Updated: 2023/12/22 17:27:52 by diosanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_operator	get_operator(char *operator)
{
	t_operator	op;

	if (!operator)
		op = NONE;
	else if (streq(operator, "|"))
		op = PIPE;
	else if (streq(operator, ">>"))
		op = RDR_OUT_APPEND;
	else if (streq(operator, ">"))
		op = RDR_OUT_REPLACE;
	else if (streq(operator, "<<"))
		op = RDR_INPUT_UNTIL;
	else if (streq(operator, "<"))
		op = RDR_INPUT;
	else
		op = NONE;
	free(operator);
	return (op);
}

size_t	get_ac(char **parsed)
{
	size_t	i;

	i = 0;
	while (parsed[i])
		i++;
	return (i);
}

size_t	get_token_len(char *input_at_i)
{
	size_t	i;
	char	quotes;

	i = 0;
	if (is_onstr(OPERATORS, input_at_i[i]))
	{
		if (input_at_i[i] == input_at_i[i + 1])
			return (2);
		return (1);
	}
	while (input_at_i[i]
		&& !is_spaces(input_at_i[i])
		&& !is_onstr(OPERATORS, input_at_i[i]))
	{
		if (is_onstr(QUOTES, input_at_i[i]))
		{
			quotes = input_at_i[i++];
			while (input_at_i[i] && input_at_i[i] != quotes)
				i++;
		}
		i++;
	}
	return (i);
}

size_t	get_nr_statements(char *input)
{
	size_t	c;
	bool	flag;
	bool	quotes;

	c = 0;
	flag = false;
	quotes = false;
	while (*input)
	{
		if (is_onstr(OPERATORS, *input))
			c++;
		if (is_onstr(QUOTES, *input) && *input == *(input + 1))
			input++;
		else if (is_onstr(QUOTES, *input))
			quotes = !quotes;
		if (*input != ' ' && !is_onstr(OPERATORS, *input) && !flag && !quotes)
		{
			flag = true;
			c++;
		}
		else if (*input == ' ' || is_onstr(OPERATORS, *input))
			flag = false;
		input++;
	}
	return (c);
}
