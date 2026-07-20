/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   031_utils_4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrager <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 08:01:57 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/13 23:24:24 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	size_t	slen;
	char	*sub;

	if (!s)
		return (NULL);
	slen = 0;
	while (s[slen])
		slen++;
	if (start >= slen)
	{
		sub = malloc(1);
		if (!sub)
			return (NULL);
		sub[0] = '\0';
		return (sub);
	}
	if (start + len > slen)
		len = slen - start;
	return (ft_substr_copy(s, start, len));
}

char	*ft_substr_copy(const char *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*sub;

	sub = malloc(len + 1);
	if (!sub)
		return (NULL);
	i = 0;
	while (i < len)
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}

char	*ft_itoa(int n)
{
	long	nb;
	int		len;
	char	*str;

	nb = n;
	len = 0;
	if (nb <= 0)
		len = 1;
	len += ft_itoa_count_digits(nb);
	str = malloc(len + 1);
	if (!str)
		return (NULL);
	str[len] = '\0';
	if (nb == 0)
	{
		str[0] = '0';
		return (str);
	}
	if (nb < 0)
	{
		str[0] = '-';
		nb = -nb;
	}
	ft_itoa_fill(str, nb, len);
	return (str);
}

int	ft_itoa_count_digits(long nb)
{
	long	tmp;
	int		count;

	tmp = nb;
	count = 0;
	while (tmp != 0)
	{
		tmp /= 10;
		count++;
	}
	return (count);
}

void	ft_itoa_fill(char *str, long nb, int len)
{
	while (nb > 0)
	{
		str[--len] = (nb % 10) + '0';
		nb /= 10;
	}
}
