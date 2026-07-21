/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_wildcard_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrelli <melchior.grellier42@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 06:17:19 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/21 01:39:05 by megrelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell_bonus.h"

static int	expand_process_one(char *pattern,
	t_wildlist *tmp, char **result, int *found)
{
	char	*joined;

	if (wild_match(pattern, tmp->name, 0, 0))
	{
		if (tmp->name[0] == '.' && pattern[0] != '.')
			return (0);
		*found = 1;
		joined = ft_strjoin(*result, tmp->name);
		free(*result);
		*result = joined;
		joined = ft_strjoin(*result, " ");
		free(*result);
		*result = joined;
	}
	return (0);
}

char	*expand_collect_matches(char *pattern, t_wildlist *files, int *found)
{
	t_wildlist	*tmp;
	char		*result;

	tmp = files;
	result = ft_strdup("");
	*found = 0;
	while (tmp)
	{
		if (tmp->name[0] == '.' && pattern[0] != '.'
			&& wild_match(pattern, tmp->name, 0, 0))
		{
			tmp = tmp->next;
			continue ;
		}
		expand_process_one(pattern, tmp, &result, found);
		tmp = tmp->next;
	}
	return (result);
}

char	*expand_join_tab(char **tab)
{
	char	*result;
	char	*joined;
	int		i;

	result = ft_strdup("");
	i = 0;
	while (tab[i])
	{
		joined = ft_strjoin(result, tab[i]);
		free(result);
		result = joined;
		if (tab[i + 1])
		{
			joined = ft_strjoin(result, " ");
			free(result);
			result = joined;
		}
		i++;
	}
	return (result);
}

char	*expand_sort_result(char *result)
{
	char	**tab;
	char	*final;

	tab = ft_split(result, ' ');
	free(result);
	bubble_sort(tab);
	final = expand_join_tab(tab);
	free_split(tab);
	return (final);
}
