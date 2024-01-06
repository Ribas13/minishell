/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diosanto <diosanto@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 15:13:38 by diosanto          #+#    #+#             */
/*   Updated: 2022/10/24 16:20:15 by diosanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <string.h>

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*p;

	p = s;
	while (n > 0)
	{
	*p = c;
	p++;
	n--;
	}
	return (s);
}
/*
int	main(void)
{
	char	*str;

	str = strdup("hello111111111");
	ft_memset(str, 'a', 14);
	printf("%s\n", str);
}*/
