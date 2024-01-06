/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diosanto <diosanto@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 16:35:34 by diosanto          #+#    #+#             */
/*   Updated: 2023/12/05 17:09:20 by diosanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Iterates through the list, frees memory for each node's
variable name and value, then frees the memory for the node
itself */
void	v_lstclear(t_vlst **head)
{
	t_vlst	*temp;
	t_vlst	*next_node;

	if (!head)
		return ;
	temp = *head;
	while (temp != NULL)
	{
		next_node = temp->next;
		free(temp->var_name);
		free(temp->var_value);
		free(temp);
		temp = next_node;
	}
	*head = NULL;
}

/* Clears the command history, and if the environment variables list
(envp_lst) and parsed statement list(data->head) are not NULL, functions
are called to clean each respective list

1.Clear the command history
2. If envp_lst is not NULL, clean it
3. If head(parsed element list) is not NULL, clean it
 */
void	destroy(t_data *data)
{
	rl_clear_history();
	if (data->envp_lst)
		v_lstclear(&data->envp_lst);
	if (data->head)
		p_lstclear(&data->head);
}
