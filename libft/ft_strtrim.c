/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrelli <melchior.grellier42@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 10:19:14 by megrelli          #+#    #+#             */
/*   Updated: 2025/11/12 10:19:14 by megrelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_isset(char c, const char *set)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(const char *s1, const char *set)
{
	char	*s2;
	size_t	start;
	size_t	end;
	size_t	i;

	if (!set)
		return (ft_strdup(s1));
	if (!s1)
		return (NULL);
	start = 0;
	end = ft_strlen(s1);
	while (s1[start] && ft_isset(s1[start], set))
		start++;
	while (end > start && ft_isset(s1[end - 1], set))
		end--;
	s2 = malloc(sizeof(char) * (end - start + 1));
	if (!s2)
		return (NULL);
	i = 0;
	while (start < end)
		s2[i++] = s1[start++];
	s2[i] = '\0';
	return (s2);
}

/*int main()
{
	char *s1 = "abcdef     Ninho     cdaebf";
	char *set = "fedbc a";
	printf("%s", ft_strtrim(s1, set));
}*/