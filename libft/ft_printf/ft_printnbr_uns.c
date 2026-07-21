/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printnbr_uns.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrelli <melchior.grellier42@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 15:42:35 by megrelli          #+#    #+#             */
/*   Updated: 2025/12/30 18:12:34 by megrelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	ft_nbrlen_uns(unsigned int nb)
{
	int	count;

	count = 0;
	if (nb == 0)
		return (1);
	while (nb != 0)
	{
		count++;
		nb /= 10;
	}
	return (count);
}

static char	*ft_itoa_uns(unsigned int nb)
{
	char	*str;
	int		len;

	len = ft_nbrlen_uns(nb);
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	str[len] = '\0';
	if (nb == 0)
	{
		str[0] = '0';
		return (str);
	}
	while (nb != 0 && len--)
	{
		str[len] = (nb % 10) + '0';
		nb /= 10;
	}
	return (str);
}

int	ft_printnbr_uns(unsigned int nb)
{
	char	*str;
	int		len;

	str = ft_itoa_uns(nb);
	len = ft_printstr(str);
	free(str);
	return (len);
}
