/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   001_bonus_wildcard_1.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrager <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 06:16:18 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/20 06:16:33 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include_bonus/minishell.h"

int	has_unquoted_wildcard(char *s)
{
	int	i;
	int	in_single;
	int	in_double;

	i = 0;
	in_single = 0;
	in_double = 0;
	while (s[i])
	{
		if (s[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (s[i] == '"' && !in_single)
			in_double = !in_double;
		else if (s[i] == '*' && !in_single && !in_double)
			return (1);
		i++;
	}
	return (0);
}

char	*expand_wildcard(char *pattern)
{
	t_wildlist	*files;
	char		*result;
	int			found;

	files = wild_read_directory();
	result = expand_collect_matches(pattern, files, &found);
	wild_clear(&files);
	if (!found)
	{
		free(result);
		return (ft_strdup(pattern));
	}
	if (result[0] && result[ft_strlen(result) - 1] == ' ')
		result[ft_strlen(result) - 1] = '\0';
	return (expand_sort_result(result));
}

t_wildlist	*wild_read_directory(void)
{
	DIR				*dir;
	struct dirent	*entry;
	t_wildlist		*lst;

	lst = NULL;
	dir = opendir(".");
	if (!dir)
		return (NULL);
	entry = readdir(dir);
	while (entry)
	{
		wild_add_back(&lst, entry->d_name);
		entry = readdir(dir);
	}
	closedir(dir);
	return (lst);
}

void	wild_add_back(t_wildlist **lst, char *name)
{
	t_wildlist	*new;
	t_wildlist	*tmp;

	new = malloc(sizeof(t_wildlist));
	if (!new)
		return ;
	new->name = ft_strdup(name);
	new->next = NULL;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

void	wild_clear(t_wildlist **lst)
{
	t_wildlist	*cur;
	t_wildlist	*next;

	cur = *lst;
	while (cur)
	{
		next = cur->next;
		free(cur->name);
		free(cur);
		cur = next;
	}
	*lst = NULL;
}
