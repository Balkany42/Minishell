/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   021_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrager <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 02:35:52 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/13 14:30:26 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include_bonus/minishell.h"

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return ;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*tmp;
	int		i;

	while (cmds)
	{
		tmp = cmds->next;
		if (cmds->args)
		{
			i = 0;
			while (cmds->args[i])
			{
				free(cmds->args[i]);
				i++;
			}
			free(cmds->args);
		}
		free_redirs(cmds->redirs);
		free(cmds);
		cmds = tmp;
	}
}

void	free_redirs(t_redir *r)
{
	t_redir	*next;

	while (r)
	{
		next = r->next;
		if (r->file)
			free(r->file);
		free(r);
		r = next;
	}
}

void	free_env_list(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
}
