/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diosanto <diosanto@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 15:13:27 by diosanto          #+#    #+#             */
/*   Updated: 2022/11/09 17:32:02 by diosanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <string.h>

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	void		*d;
	const void	*s;
	size_t		i;

	d = dest;
	s = src;
	i = 0;
	if (dest == src || n == 0)
		return (dest);
	while (n > 0)
	{
		((unsigned char *)d)[i] = ((unsigned char *)s)[i];
		i++;
		n--;
	}
	return (d);
}
/*
int	main(void)
{
	char	*src;
	char	*dest;

	src = strdup("hello111111111");
	dest = strdup("makie");
	ft_memcpy(dest, src, 14);
	printf("%s\n", dest);
}*/
