/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diosanto <diosanto@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 15:37:39 by diosanto          #+#    #+#             */
/*   Updated: 2022/11/09 16:16:50 by diosanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*plzwork;

	if (!lst || !*lst)
		return ;
	while (*lst)
	{
		plzwork = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = plzwork;
	}
	lst = NULL;
}
