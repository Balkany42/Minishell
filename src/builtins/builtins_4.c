/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrelli <melchior.grellier42@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 04:25:55 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/21 00:46:06 by megrelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	print_export_entry(char *entry)
{
	char	*eq;

	eq = ft_strchr(entry, '=');
	if (eq)
	{
		write(1, "declare -x ", 11);
		write(1, entry, eq - entry + 1);
		write(1, "\"", 1);
		write(1, eq + 1, ft_strlen(eq + 1));
		write(1, "\"\n", 2);
	}
	else
	{
		write(1, "declare -x ", 11);
		write(1, entry, ft_strlen(entry));
		write(1, "\n", 1);
	}
}

static void	print_export_sorted(t_env *env)
{
	char	**tab;
	int		i;

	tab = env_to_tab(env);
	i = 0;
	if (!tab)
		return ;
	sort_env(tab);
	while (tab[i])
	{
		print_export_entry(tab[i]);
		i++;
	}
	i = 0;
	while (tab[i])
		free(tab[i++]);
	free(tab);
}

int	builtin_export(t_cmd *cmd, t_minishell *sh)
{
	int	i;

	i = 1;
	if (!cmd->args[1])
	{
		print_export_sorted(sh->env);
		sh->exit_status = 0;
		return (0);
	}
	while (cmd->args[i])
	{
		export_process_one(cmd->args[i], sh);
		i++;
	}
	sh->exit_status = 0;
	return (0);
}

int	is_valid_identifier(char *s)
{
	int	i;

	if (!s || (!ft_isalpha(s[0]) && s[0] != '_'))
		return (0);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
