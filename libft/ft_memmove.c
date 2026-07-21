/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrelli <melchior.grellier42@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 16:33:51 by megrelli          #+#    #+#             */
/*   Updated: 2025/11/10 16:33:51 by megrelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*tmp_d;
	unsigned char	*tmp_s;

	if (n == 0)
		return (dest);
	if (!dest && !src)
		return (NULL);
	tmp_d = (unsigned char *) dest;
	tmp_s = (unsigned char *) src;
	if (tmp_d > tmp_s)
	{
		while (n-- > 0)
			tmp_d[n] = tmp_s[n];
	}
	else
		ft_memcpy(tmp_d, tmp_s, n);
	return (dest);
}

/*int main()
{
    char source[] = "ABCDE";
    char src[] = "ABCDE";

    printf("\\\\ MA FONCTION \\\\ \n");
    printf("%s\n", source);
    ft_memmove(source + 2, source, 3);
    printf("%s\n", source);

    printf("\\\\ LA VRAIE \\\\ \n");
    printf("%s\n", src);
    memmove(src + 2, src, 3);
    printf("%s", src);
    return (0);
}*/