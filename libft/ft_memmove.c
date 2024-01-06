/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diosanto <diosanto@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 15:14:43 by diosanto          #+#    #+#             */
/*   Updated: 2022/11/02 14:27:54 by diosanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*char_dest;
	unsigned char	*char_src;

	char_dest = ((unsigned char *)dest);
	char_src = ((unsigned char *)src);
	i = 0;
	if (dest > src)
	{
		while (i < n)
		{
			char_dest[n - 1 - i] = char_src[n - 1 - i];
			i++;
		}
	}
	else
	{
		ft_memcpy(char_dest, char_src, n);
	}
	return (dest);
}
