/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   032_utils_5.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrager <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 08:06:54 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/17 00:50:46 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include_bonus/minishell.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

int	count_digits(int n)
{
	int	count;

	count = 1;
	while (n / 10 != 0)
	{
		n /= 10;
		count++;
	}
	return (count);
}

int	skip_spaces(char *s, int i)
{
	while (s[i] == ' ' || s[i] == '\t')
		i++;
	return (i);
}

char	*str_join_and_free(char *a, char *b)
{
	char	*res;

	res = ft_strjoin(a, b);
	free(a);
	free(b);
	return (res);
}
