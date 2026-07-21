/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_5.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrelli <melchior.grellier42@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 08:06:54 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/20 23:15:59 by megrelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell_bonus.h"

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

size_t	ft_strlen_2(const char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i] && s[i] != ' ' && s[i] != '\n' && s[i] != '\t')
		i++;
	return (i);
}
