/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   025_builtins_3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrager <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 04:01:45 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/13 04:07:44 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	builtin_env(char **args, t_minishell *sh)
{
	t_env	*e;

	(void)args;
	e = sh->env;
	while (e)
	{
		if (e->value)
			printf("%s=%s\n", e->key, e->value);
		e = e->next;
	}
	sh->exit_status = 0;
	return (0);
}

int	builtin_unset(t_cmd *cmd, t_minishell *sh)
{
	int	i;

	i = 1;
	if (!cmd->args[1])
	{
		sh->exit_status = 0;
		return (0);
	}
	while (cmd->args[i])
	{
		unset_process_one(cmd->args[i], sh);
		i++;
	}
	sh->exit_status = 0;
	return (0);
}

void	unset_process_one(char *key, t_minishell *sh)
{
	if (unset_is_invalid(key, sh))
		return ;
	unset_remove_key(&sh->env, key);
}

int	unset_is_invalid(char *key, t_minishell *sh)
{
	if (!is_valid_identifier(key))
	{
		printf("minishell: unset: `%s`: not a valid identifier\n", key);
		sh->exit_status = 1;
		return (1);
	}
	return (0);
}

void	unset_remove_key(t_env **env, char *key)
{
	t_env	*cur;
	t_env	*prev;

	cur = *env;
	prev = NULL;
	while (cur)
	{
		if (!ft_strcmp(cur->key, key))
		{
			if (prev)
				prev->next = cur->next;
			else
				*env = cur->next;
			free(cur->key);
			free(cur->value);
			free(cur);
			return ;
		}
		prev = cur;
		cur = cur->next;
	}
}
