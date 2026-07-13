/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   024_builtins_5.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrager <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 04:30:58 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/13 04:56:39 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	export_process_one(char *arg, t_minishell *sh)
{
	char	*eq;
	char	*key;
	char	*value;

	eq = ft_strchr(arg, '=');
	key = export_extract_key(arg, eq);
	if (!is_valid_identifier(key))
	{
		printf("minishell: export: `%s`: not a valid identifier\n", arg);
		free(key);
		sh->exit_status = 1;
		return ;
	}
	if (!eq)
	{
		env_set(&sh->env, key, "");
		free(key);
		return ;
	}
	value = ft_strdup(eq + 1);
	env_set(&sh->env, key, value);
	free(key);
	free(value);
}

char	*export_extract_key(char *arg, char *eq)
{
	if (eq)
		return (ft_substr(arg, 0, eq - arg));
	return (ft_strdup(arg));
}

void	env_set(t_env **env, char *key, char *value)
{
	t_env	*new;

	if (env_set_update(*env, key, value))
		return ;
	new = malloc(sizeof(t_env));
	if (!new)
		return ;
	new->key = ft_strdup(key);
	if (value)
		new->value = ft_strdup(value);
	else
		new->value = "";
	new->next = *env;
	*env = new;
}

int	env_set_update(t_env *env, char *key, char *value)
{
	t_env	*e;

	e = env;
	while (e)
	{
		if (!ft_strcmp(e->key, key))
		{
			free(e->value);
			if (value)
				e->value = ft_strdup(value);
			else
				e->value = ft_strdup("");
			return (1);
		}
		e = e->next;
	}
	return (0);
}

void	sort_env(char **tab)
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
			if (ft_strcmp(tab[i], tab[j]) > 0)
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
