/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diosanto <diosanto@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 18:14:06 by diosanto          #+#    #+#             */
/*   Updated: 2022/11/15 10:13:00 by diosanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

static int	word_size(char const *s, char c, size_t i)
{
	size_t	size;

	size = 0;
	while (s[i] != c && s[i])
	{
		size++;
		i++;
	}
	return (size);
}

static size_t	word_counter(char const *s, char c)
{
	size_t	i;
	size_t	index;

	index = 0;
	i = 0;
	while (s[index] != '\0')
	{
		if (s[index] != c && (s[index + 1] == c || s[index + 1] == '\0'))
		{
			i++;
		}
		index++;
	}
	return (i);
}

char	**ft_split(char const *s, char c)
{
	size_t	i;
	size_t	size;
	size_t	words;
	char	**tab;

	i = 0;
	tab = malloc((word_counter(s, c) + 1) * sizeof(char *));
	if (!tab)
		return (NULL);
	words = 0;
	while (words < word_counter(s, c))
	{
		while (s[i] == c)
			i++;
		size = word_size(s, c, i);
		tab[words] = ft_substr(s, i, word_size(s, c, i));
		i += size;
		words++;
	}
	tab[words] = 0;
	return (tab);
}
