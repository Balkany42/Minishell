/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printhexa.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrelli <melchior.grellier42@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 18:35:21 by megrelli          #+#    #+#             */
/*   Updated: 2025/12/30 18:12:26 by megrelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	ft_hexalen(unsigned long long nb)
{
	int	len;

	if (nb == 0)
		return (1);
	len = 0;
	while (nb != 0)
	{
		len++;
		nb /= 16;
	}
	return (len);
}

static void	ft_puthexa(unsigned int nb, char format)
{
	if (nb >= 16)
	{
		ft_puthexa(nb / 16, format);
		ft_puthexa(nb % 16, format);
	}
	else if (nb <= 9)
		ft_printchar(nb + '0');
	else
	{
		if (format == 'x')
			ft_printchar(nb - 10 + 'a');
		if (format == 'X')
			ft_printchar(nb - 10 + 'A');
	}
}

int	ft_printhexa(unsigned int nb, char format)
{
	ft_puthexa(nb, format);
	return (ft_hexalen(nb));
}

/*int main() 
{ 
	ft_printhexa(17, 'x');
	write(1, "\n", 1);
	ft_printhexa(250, 'X');
	write(1, "\n", 1);
	ft_printhexa(13, 'x');
	write(1, "\n", 1);
	ft_printhexa(45637, 'X');
	write(1, "\n", 1);
}*/