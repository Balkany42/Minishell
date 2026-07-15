/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   017_executor_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrager <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 01:26:48 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/13 23:31:59 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	exec_single(t_cmd *cmd, t_minishell *sh)
{
	pid_t	pid;

	if (is_builtin(cmd->args[0]))
		return (exec_single_builtin(cmd, sh));
	pid = fork();
	if (pid == -1)
		return ;
	if (pid == 0)
		exec_single_child(cmd, sh);
	exec_single_parent(pid, sh);
}

void	exec_single_builtin(t_cmd *cmd, t_minishell *sh)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (apply_redirs(cmd) == -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		return ;
	}
	exec_builtin(cmd, sh);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

// void	exec_single_child(t_cmd *cmd, t_minishell *sh)
// {
// 	signal(SIGINT, SIG_DFL);
// 	signal(SIGQUIT, SIG_DFL);
// 	if (apply_redirs(cmd) == -1)
// 		exit(1);
// 	exec_external(cmd, sh);
// 	exit(127);
// }

void    exec_single_child(t_cmd *cmd, t_minishell *sh)
{
	signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);

    // 🔹 Commande vide avec redirections
    if (!cmd->args || !cmd->args[0])
    {
        if (cmd->redirs)
        {
            if (apply_redirs(cmd) == -1)
                exit(1);

            // 🔥 Copie STDIN → STDOUT pour écrire le heredoc dans le fichier
            char buffer[4096];
            ssize_t n;

            while ((n = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0)
                write(STDOUT_FILENO, buffer, n);
        }
        exit(0);
    }

    if (apply_redirs(cmd) == -1)
        exit(1);

    exec_external(cmd, sh);
    exit(127);
}



void	exec_single_parent(pid_t pid, t_minishell *sh)
{
	int	status;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &status, 0);
	init_signals();
	if (WIFEXITED(status))
		sh->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		sh->exit_status = 128 + WTERMSIG(status);
}
