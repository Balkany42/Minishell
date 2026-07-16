/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   011_parser_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrager <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 00:33:23 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/13 23:32:44 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// t_cmd	*parse_command(t_token **list, t_minishell *sh)
// {
// 	t_cmd	*cmd;
// 	t_token	*tok;

// 	cmd = cmd_new();
// 	tok = *list;
// 	cmd->redirs = NULL;
// 	while (tok && tok->type != PIPE)
// 	{
// 		if (tok->type == WORD)
// 			tok = parse_word_token(cmd, tok, sh);
// 		else if (tok->type == REDIR_IN || tok->type == REDIR_OUT
// 			|| tok->type == APPEND || tok->type == HEREDOC)
// 			tok = parse_redirection_token(cmd, tok, sh);
// 		else
// 			return (free_cmds(cmd), NULL);
// 	}
// 	*list = tok;
// 	return (cmd);
// }

t_cmd *parse_command(t_token **list, t_minishell *sh)
{
    t_cmd *cmd = cmd_new();
    t_token *tok = *list;

	//printf("parse_command: start\n");
    while (tok && tok->type != PIPE)
    {
		//printf("  tok type=%d, value='%s'\n", tok->type, tok->value);
        if (tok->type == WORD)
        {
            tok = parse_word_token(cmd, tok, sh);
			//if(!tok)
				//printf("  parse_word_token returned NULL\n");
            // if (!tok)            // 🔥 AJOUTER
            //     return NULL;     // 🔥 STOPPER
        }
        else if (tok->type == REDIR_IN || tok->type == REDIR_OUT
              || tok->type == APPEND || tok->type == HEREDOC)
        {
            //printf("  entering parse_redirection_token\n");
			tok = parse_redirection_token(cmd, tok, sh);
            if (!tok)
			{
				if(sh->redir_error)
					return(NULL);//printf("  parse_redirection_token returned NULL\n");
				break ;
			}
                
        }
        else
        {
            //printf("  unexpected token type=%d\n", tok->type);
			free_cmds(cmd);
            return NULL;
        }
    }

    *list = tok;
	//printf("parse_command: done, cmd args=%s, redirs=%p\n",
    //       cmd->args && cmd->args[0] ? cmd->args[0] : "(null)",
    //       (void*)cmd->redirs);
    return cmd;
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
	//printf("    expanded='%s'\n", expanded ? expanded : "(null)");
	if (is_ambiguous_redirect(type, expanded))
		return (redir_error(cmd, tok, expanded, sh), NULL);
	add_redir(cmd, type, expanded);
	//printf("    is_ambiguous_redirect=%d\n", is_ambiguous_redirect(type, expanded));
	return (tok->next);
}

int	is_ambiguous_redirect(t_tokentype type, char *expanded)
{
	//printf("      checking ambiguous: expanded='%s'\n", expanded);
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
	//expanded = expanded;
	//cmd = cmd;
	free_cmds(cmd);
}
