/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   003_env_to_tab.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrager <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 01:20:07 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/13 01:22:04 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**env_to_tab(t_env *env)
{
	char	**tab;
	t_env	*tmp;
	int		size;
	int		i;

	size = env_count(env);
	tab = malloc(sizeof(char *) * (size + 1));
	if (!tab)
		return (NULL);
	tmp = env;
	i = 0;
	while (tmp)
	{
		tab[i] = env_build_entry(tmp);
		i++;
		tmp = tmp->next;
	}
	tab[i] = NULL;
	return (tab);
}

int	env_count(t_env *env)
{
	int		count;
	t_env	*tmp;

	count = 0;
	tmp = env;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

char	*env_build_entry(t_env *var)
{
	char	*tmp;
	char	*res;

	if (var->value)
	{
		tmp = ft_strjoin(var->key, "=");
		res = ft_strjoin(tmp, var->value);
		free(tmp);
	}
	else
		res = ft_strdup(var->key);
	return (res);
}
