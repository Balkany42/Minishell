/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   011_parser_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrager <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 00:33:23 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/16 23:18:34 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_cmd	*parse_command(t_token **list, t_minishell *sh)
{
	t_cmd	*cmd;
	t_token	*tok;

	cmd = cmd_new();
	tok = *list;
	while (tok && tok->type != PIPE)
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
	*list = tok;
	return (cmd);
}

t_token	*parse_word_token(t_cmd *cmd, t_token *tok, t_minishell *sh)
{
	char	*expanded;

	expanded = expand_token(tok, sh);
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
