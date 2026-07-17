/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   013_expander.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrager <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 13:29:59 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/16 21:25:58 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include_bonus/minishell.h"

t_env	*init_env(char **envp)
{
	t_env	*env;
	int		i;
	char	*eq;
	char	*key;
	char	*value;

	env = NULL;
	i = 0;
	while (envp[i])
	{
		eq = ft_strchr(envp[i], '=');
		if (!eq)
		{
			i++;
			continue ;
		}
		key = ft_substr(envp[i], 0, eq - envp[i]);
		value = ft_strdup(eq + 1);
		add_env_back(&env, new_env_node(key, value));
		i++;
	}
	return (env);
}

void	add_env_back(t_env **env, t_env *new)
{
	t_env	*tmp;

	if (!*env)
	{
		*env = new;
		return ;
	}
	tmp = *env;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

t_env	*new_env_node(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = key;
	node->value = value;
	node->next = NULL;
	return (node);
}

char    *expand_token(t_token *tok, t_minishell *sh)
{
    char *expanded;
	char *wild;

    if (tok->quote_type == SINGLE_QUOTE)
        return (ft_strdup(tok->value));

    expanded = expand_word(tok->value, sh);

    // 👉 ICI : tu détectes et appliques les wildcards
    if (tok->quote_type == NO_QUOTE && has_unquoted_wildcard(expanded))
    {
        wild = expand_wildcard(expanded);
        free(expanded);          // 🔥 on libère la chaîne produite par expand_word
        return (wild);
    }

    return expanded;
}

char	*env_get(t_env *env, char *key)
{
	while (env)
	{
		if (!ft_strcmp(env->key, key))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}
