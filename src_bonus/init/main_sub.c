/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_sub.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrelli <melchior.grellier42@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 20:57:36 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/21 01:52:54 by megrelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell_bonus.h"

static int	handle_lexer_and_syntax(char *line, t_token **tokens,
		t_minishell *sh)
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

static void	process_line(char *line, t_minishell *sh)
{
	t_token	*tokens;
	t_ast	*root;
	t_token	*cursor;

	if (!is_blank_line(line))
		add_history(line);
	if (!handle_lexer_and_syntax(line, &tokens, sh))
		return (free(line));
	cursor = tokens;
	root = parse_and_or(&cursor, sh);
	free_tokens(tokens);
	if (!root)
		return (free(line));
	prepare_heredocs_ast(root, sh);
	if (sh->heredoc_interrupted)
	{
		sh->heredoc_interrupted = 0;
		free_ast(root);
		return (free(line));
	}
	exec_ast(root, sh);
	free_ast(root);
	free(line);
	free_tab(sh->envp);
	sh->envp = env_to_tab(sh->env);
}

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

void	cleanup_minishell(t_minishell *sh)
{
	free_env_list(sh->env);
	free_tab(sh->envp);
}
