/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrelli <melchior.grellier42@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:46:57 by megrelli          #+#    #+#             */
/*   Updated: 2025/11/10 14:46:57 by megrelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	unsigned char	*tab;
	size_t			total_size;

	if (size != 0 && count > (SIZE_MAX / size))
		return (NULL);
	total_size = count * size;
	tab = malloc(total_size);
	if (!tab)
		return (NULL);
	ft_bzero(tab, total_size);
	return (tab);
}
