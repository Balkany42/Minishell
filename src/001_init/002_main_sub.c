/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   002_main_sub.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrager <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 20:57:36 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/16 21:27:27 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	init_minishell(t_minishell *sh, char **envp)
{
	sh->env = init_env(envp);
	sh->envp = env_to_tab(sh->env);
	sh->exit_status = 0;
	sh->heredoc_interrupted = 0;
	sh->should_exit = 0;
	sh->redir_error = 0;
	init_signals();
}

void	run_minishell_loop(t_minishell *sh)
{
	char	*line;

	while (1)
	{
		line = readprompt();
		if (!line)
		{
			write (1, "exit\n", 5);
			break ;
		}
		process_line(line, sh);
		if (sh->should_exit)
			break ;
	}
}

void	process_line(char *line, t_minishell *sh)
{
	t_token	*tokens;
	t_cmd	*cmds;

	if (!is_blank_line(line))
		add_history(line);
	if (!handle_lexer_and_syntax(line, &tokens, sh))
		return (free(line));
	cmds = parser(tokens, sh);
	free_tokens(tokens);
	if (!cmds)
		return (free (line));
	prepare_heredocs(cmds, sh);
	if (sh->heredoc_interrupted)
	{
		sh->heredoc_interrupted = 0;
		free_cmds(cmds);
		free(line);
		return ;
	}
	executor(cmds, sh);
	free_cmds(cmds);
	free(line);
	free_tab(sh->envp);
	sh->envp = env_to_tab(sh->env);
}

int	handle_lexer_and_syntax(char *line, t_token **tokens, t_minishell *sh)
{
	*tokens = lexer(line);
	if (!*tokens)
	{
		if (line[0] != '\0' && line[0] != ' ' && line[0] != '\t')
			printf("Syntax error: unclosed quote\n");
		sh->exit_status = 2;
		return (0);
	}
	if (check_syntax_errors(*tokens))
	{
		printf("Syntax error near token\n");
		sh->exit_status = 2;
		free_tokens(*tokens);
		return (0);
	}
	return (1);
}

void	cleanup_minishell(t_minishell *sh)
{
	free_env_list(sh->env);
	free_tab(sh->envp);
}
