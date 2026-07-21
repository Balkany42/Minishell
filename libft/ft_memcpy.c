/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrelli <melchior.grellier42@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 15:50:10 by megrelli          #+#    #+#             */
/*   Updated: 2025/11/10 15:50:10 by megrelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*tmp_d;
	unsigned char	*tmp_s;

	if (n == 0)
		return (dest);
	if (!dest && !src)
		return (NULL);
	tmp_d = (unsigned char *) dest;
	tmp_s = (unsigned char *) src;
	i = 0;
	while (i < n)
	{
		tmp_d[i] = tmp_s[i];
		i++;
	}
	return (dest);
}
/*int main()
{
    char source[] = "c'est ninho";
    char destination[20] = "salut";

    char src[] = "c'est ninho";
    char dest[20] = "salut";

    printf("\\\\ MA FONCTION \\\\ \n");
    printf("%s\n", destination);
    ft_memcpy(destination, source, strlen(source) + 1);
    printf("%s\n", destination);

    printf("\\\\ LA VRAIE \\\\ \n");
    printf("%s\n", dest);
    memcpy(dest, src, strlen(src) + 1);
    printf("%s\n", dest);
    return (0);
}*/