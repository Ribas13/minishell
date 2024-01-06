/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_envp_lst.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diosanto <diosanto@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 17:09:51 by diosanto          #+#    #+#             */
/*   Updated: 2024/01/03 00:04:59 by diosanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Unsets the existing "OLDPWD" variable, creates a new value by combining
"OLDPWD=" with the user's home dir, saves the new value in the env vars list,
and then frees the mem allocated for temporary string

1. Unset the "OLDPWD" env var
2. Create a new value for "OLDPWD" with the user's home dir
3. Save the new "OLDPWD" value in the env var list
4. Free temp */
/* static void	init_oldpwd(t_vlst **head)
{
	char	*temp;

	unset_var("OLDPWD", head);
	temp = ft_strjoin("OLDPWD=", getenv("HOME"));
	save_user_vars(temp, head, true);
	free(temp);
}
 */
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
	//init_oldpwd(&head);
	return (head);
}
