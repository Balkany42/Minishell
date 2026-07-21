/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrelli <melchior.grellier42@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 09:09:22 by megrelli          #+#    #+#             */
/*   Updated: 2025/11/12 09:09:22 by megrelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (little[i] == '\0')
		return ((char *) big);
	while (big[i] && i < len)
	{
		j = 0;
		while ((big[i + j] == little[j]) && big[i + j] && (i + j < len))
		{
			if (little[j + 1] == '\0')
				return ((char *) big + i);
			j++;
		}
		i++;
	}
	return (NULL);
}

/*int main()
{
	char *s1 = "Follow the white rabbit";
	char *s2 = "bit";
	printf("%s", ft_strnstr(s1, s2, 24));
}*/