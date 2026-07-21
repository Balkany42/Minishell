/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_wildcard_3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrelli <melchior.grellier42@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 06:18:07 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/21 01:39:30 by megrelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell_bonus.h"

static int	strcmp_bash(const char *a, const char *b, int index)
{
	char	ca;
	char	cb;
	char	la;
	char	lb;

	while (a[index] && b[index])
	{
		ca = a[index];
		cb = b[index];
		if (ca != cb)
		{
			if (ft_isalpha(ca) && ft_isalpha(cb))
			{
				la = ft_tolower(ca);
				lb = ft_tolower(cb);
				if (la != lb)
					return (la - lb);
				return (ca - cb);
			}
			return (ca - cb);
		}
		index++;
	}
	return (a[index] - b[index]);
}

void	bubble_sort(char **tab)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (tab[i])
	{
		j = i + 1;
		while (tab[j])
		{
			if (strcmp_bash(tab[i], tab[j], 0) > 0)
			{
				tmp = tab[i];
				tab[i] = tab[j];
				tab[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

int	wild_match(char *pattern, char *name, int i, int j)
{
	while (pattern[i] && name[j])
	{
		if (pattern[i] == '*')
		{
			if (!pattern[i + 1])
				return (1);
			while (name[j] && name[j] != pattern[i + 1])
				j++;
			i++;
		}
		else if (pattern[i] == name[j])
		{
			i++;
			j++;
		}
		else
			return (0);
	}
	if (pattern[i] == '*' && !pattern[i + 1])
		return (1);
	return (pattern[i] == name[j]);
}

void	free_split(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}
