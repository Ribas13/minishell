/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_envp_lst.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diosanto <diosanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 17:09:51 by diosanto          #+#    #+#             */
/*   Updated: 2024/01/06 18:49:55 by diosanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Split each environment variable into components, creates nodes for
each variable, and links them together to form a linked list. Also
initializes the "OLDPWD" environment variable, and finally, it returns
the pointer to the head of the env list 

1. Split the first env var and get components
2. Create a new node and initialize the head
3. free line
4. LOOP
5. Split the next env var
6. Create a new node and link it to the list
7. free the mem allocated for the split components
8. move to the next node
9. LOOP END
10. Init the "OLDPWD" env var
11. Return the head*/
t_vlst	*init_envp_lst(char **envp)
{
	t_vlst	*head;
	t_vlst	*temp;
	char	**line;
	size_t	i;

	line = split_envp(envp[0]);
	head = v_new_node(line[0], line[1], true);
	free(line);
	i = 1;
	temp = head;
	while (envp[i])
	{
		line = split_envp(envp[i]);
		temp->next = v_new_node(line[0], line[1], true);
		free(line);
		temp = temp->next;
		i++;
	}
	return (head);
}
