/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysantos- <ysantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 17:47:07 by diosanto          #+#    #+#             */
/*   Updated: 2023/12/26 02:34:26 by ysantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/* Returns the string without the backslashs */
static char	*clean_value(const char *str)
{
	static char	clean[4097];
	int			i;
	int			j;

	ft_memset(clean, 0, 4096);
	if (!str || !ft_strlen(str))
		return (NULL);
	i = -1;
	j = -1;
	while (str[++i])
	{
		if (str[i] == '\\')
			++i;
		clean[++j] = str[i];
	}
	return (clean);
}

/* Iterates through the env variables list and prints each
one along with its value */
int	cmd_env(t_data *data)
{
	t_vlst	*temp;

	temp = data->envp_lst;
	while (temp != NULL)
	{
		if (temp->is_exported && temp->var_value)
			printf("%s=%s\n", temp->var_name, clean_value(temp->var_value));
		temp = temp->next;
	}
	return (EXIT_SUCCESS);
}
