/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrelli <melchior.grellier42@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 01:20:07 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/21 01:46:27 by megrelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	env_count(t_env *env)
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

static char	*env_build_entry(t_env *var)
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
