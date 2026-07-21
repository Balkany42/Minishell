/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printpointer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrelli <melchior.grellier42@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 20:53:02 by megrelli          #+#    #+#             */
/*   Updated: 2025/12/30 18:12:42 by megrelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	ft_pointerlen(uintptr_t ptr)
{
	int	len;

	len = 0;
	if (ptr == 0)
		return (1);
	while (ptr != 0)
	{
		len++;
		ptr /= 16;
	}
	return (len);
}

static void	ft_putpointer(uintptr_t ptr)
{
	if (ptr >= 16)
	{
		ft_putpointer(ptr / 16);
		ft_putpointer(ptr % 16);
	}
	else if (ptr <= 9)
		ft_printchar(ptr + '0');
	else
		ft_printchar(ptr - 10 + 'a');
}

int	ft_printpointer(void *ptr)
{
	int			len;

	len = 0;
	if (!ptr)
		return (write(1, "(nil)", 5));
	len += write(1, "0x", 2);
	ft_putpointer((uintptr_t)ptr);
	len += ft_pointerlen((uintptr_t)ptr);
	return (len);
}
