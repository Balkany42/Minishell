/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   011_parser_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrager <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 00:33:23 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/20 03:17:26 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include_bonus/minishell.h"

t_cmd	*parse_command(t_token **list, t_minishell *sh)
{
	t_cmd	*cmd;
	t_token	*tok;

	cmd = cmd_new();
	tok = *list;
	while (tok && tok->type != PIPE && tok->type != AND_IF
		&& tok->type != OR_IF && tok->type != RPAREN)
	{
		if (tok->type == WORD)
			tok = parse_word_token(cmd, tok, sh);
		else if (tok->type == REDIR_IN || tok->type == REDIR_OUT
			|| tok->type == APPEND || tok->type == HEREDOC)
		{
			tok = parse_redirection_token(cmd, tok, sh);
			if (!tok)
			{
				if (sh->redir_error)
					return (NULL);
				break ;
			}
		}
		else
			return (free_cmds(cmd), NULL);
	}
	return (*list = tok, cmd);
}

t_token	*parse_word_token(t_cmd *cmd, t_token *tok, t_minishell *sh)
{
	char	*expanded;
	char	**parts;
	int		i;

	expanded = expand_token(tok, sh);
	if (has_unquoted_wildcard(expanded))
	{
		parts = ft_split(expanded, ' ');
		free(expanded);
		i = 0;
		while (parts[i])
		{
			add_arg(cmd, ft_strdup(parts[i]));
			i++;
		}
		free_split(parts);
	}
	else
		add_arg(cmd, expanded);
	return (tok->next);
}

t_token	*parse_redirection_token(t_cmd *cmd, t_token *tok, t_minishell *sh)
{
	t_tokentype	type;
	char		*expanded;

	type = tok->type;
	tok = tok->next;
	if (!tok || tok->type != WORD)
		return (free_cmds(cmd), NULL);
	expanded = expand_token(tok, sh);
	if (has_unquoted_wildcard(tok->value))
		return (redir_error(cmd, tok, expanded, sh), NULL);
	if (is_ambiguous_redirect(type, expanded))
		return (redir_error(cmd, tok, expanded, sh), NULL);
	add_redir(cmd, type, expanded);
	return (tok->next);
}

int	is_ambiguous_redirect(t_tokentype type, char *expanded)
{
	if (type == HEREDOC)
		return (0);
	if (!expanded[0])
		return (1);
	if (ft_strchr(expanded, ' '))
		return (1);
	return (0);
}

void	redir_error(t_cmd *cmd, t_token *tok, char *expanded, t_minishell *sh)
{
	printf("minishell: %s: ambiguous redirect\n", tok->value);
	sh->exit_status = 1;
	sh->redir_error = 1;
	free(expanded);
	free_cmds(cmd);
}
