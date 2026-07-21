/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrelli <melchior.grellier42@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:47:46 by megrelli          #+#    #+#             */
/*   Updated: 2025/11/10 14:47:46 by megrelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	j;
	size_t	srclen;
	size_t	dstlen;

	i = 0;
	j = ft_strlen(dst);
	srclen = ft_strlen(src);
	dstlen = ft_strlen(dst);
	if (dstsize > 0 && dstlen < (dstsize - 1))
	{
		while (src[i] && (dstlen + i) < (dstsize - 1))
		{
			dst[j] = src[i];
			i++;
			j++;
		}
		dst[j] = '\0';
	}
	if (dstlen > dstsize)
		dstlen = dstsize;
	return (dstlen + srclen);
}
