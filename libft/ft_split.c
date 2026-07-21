/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrelli <melchior.grellier42@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 10:19:03 by megrelli          #+#    #+#             */
/*   Updated: 2025/11/12 10:19:03 by megrelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_countwords(const char *s, char c)
{
	int		words;
	int		x;
	size_t	i;

	words = 0;
	i = 0;
	while (s[i])
	{
		x = 0;
		while (s[i] == c && s[i])
			i++;
		while (s[i] != c && s[i])
		{
			if (x == 0)
			{
				words++;
				x = 1;
			}
			i++;
		}
	}
	return (words);
}

int	ft_safemalloc(char **tab, size_t index, size_t len)
{
	size_t	i;

	i = 0;
	tab[index] = malloc(sizeof(char) * (len + 1));
	if (!tab[index])
	{
		while (i < index)
		{
			free(tab[i]);
			i++;
		}
		free(tab);
		return (1);
	}
	return (0);
}

static int	ft_fillwords(char **tab, const char *s, char c)
{
	size_t	len;
	size_t	i;

	i = 0;
	while (*s)
	{
		len = 0;
		while (*s == c && *s)
			s++;
		while (*s != c && *s)
		{
			len++;
			s++;
		}
		if (len > 0)
		{
			if (ft_safemalloc(tab, i, len) == 1)
				return (1);
			ft_strlcpy(tab[i], s - len, len + 1);
		}
		i++;
	}
	return (0);
}

char	**ft_split(const char *s, char c)
{
	char	**tab;
	int		words;

	if (!s)
		return (NULL);
	words = ft_countwords(s, c);
	tab = malloc(sizeof(char *) * (words + 1));
	if (!tab)
		return (NULL);
	tab[words] = NULL;
	if (ft_fillwords(tab, s, c) == 1)
		return (NULL);
	return (tab);
}
