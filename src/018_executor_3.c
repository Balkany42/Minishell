/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   015_executor_3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrager <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 01:31:28 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/13 02:10:17 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// void	exec_pipeline(t_cmd *cmds, t_minishell *sh)
// {
// 	int		prev_fd;
// 	int		pipefd[2];
// 	pid_t	last_pid;
// 	pid_t	pid;

// 	prev_fd = -1;
// 	last_pid = -1;
// 	while (cmds)
// 	{
// 		if (cmds->next && pipe(pipefd) == -1)
// 			return ;
// 		pid = fork();
// 		if (pid == -1)
// 			return ;
// 		if (pid == 0)
// 			pipeline_child(cmds, sh, prev_fd, pipefd);
// 		else
// 		{
// 			if (!cmds->next)
// 				last_pid = pid;
// 			pipeline_parent(cmds, &prev_fd, pipefd);
// 			cmds = cmds->next;
// 		}
// 	}
// 	return (pipeline_wait_last(last_pid, sh), init_signals());
// }

void    exec_pipeline(t_cmd *cmds, t_minishell *sh)
{
    int     prev_fd;
    int     pipefd[2];
    pid_t   last_pid;
    pid_t   pid;

    prev_fd = -1;
    last_pid = -1;

    while (cmds)
    {
        // 🔥 Erreur silencieuse si pipe() échoue
        if (cmds->next && pipe(pipefd) == -1)
        {
            if (prev_fd != -1)
                close(prev_fd);
			perror("minishell: pipe");
            sh->exit_status = 1;   // ✔ bash fait ça
            return;                // ✔ ne pas appeler pipeline_wait_last()
        }

        pid = fork();
        if (pid == -1)
            return ;

        if (pid == 0)
            pipeline_child(cmds, sh, prev_fd, pipefd);
        else
        {
            signal(SIGINT, SIG_IGN);
            signal(SIGQUIT, SIG_IGN);

            if (!cmds->next)
                last_pid = pid;

            pipeline_parent(cmds, &prev_fd, pipefd);
            cmds = cmds->next;
        }
    }

    // 🔥 Ce code n'est exécuté que si pipe() n'a PAS échoué
    pipeline_wait_last(last_pid, sh);
    init_signals();
}



// void	pipeline_child(t_cmd *cmd, t_minishell *sh, int prev_fd, int pipefd[2])
// {
// 	signal(SIGINT, SIG_DFL);
// 	signal(SIGQUIT, SIG_DFL);
// 	if (prev_fd != -1)
// 	{
// 		dup2(prev_fd, STDIN_FILENO);
// 		close(prev_fd);
// 	}
// 	if (cmd->next)
// 	{
// 		close(pipefd[0]);
// 		dup2(pipefd[1], STDOUT_FILENO);
// 		close(pipefd[1]);
// 	}
// 	if (apply_redirs(cmd) == -1)
// 		exit(1);
// 	if (is_builtin(cmd->args[0]))
// 	{
// 		exec_builtin(cmd, sh);
// 		exit(sh->exit_status);
// 	}
// 	exec_external(cmd, sh);
// 	exit(127);
// }

void    pipeline_child(t_cmd *cmd, t_minishell *sh, int prev_fd, int pipefd[2])
{
	signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
	//printf(">>> pipeline_child CALLED for %s\n", cmd->args[0]);
    // 🔹 Commande vide avec redirections (cas du heredoc après un pipe)
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

    // 🔹 Gestion du pipe en entrée
    if (prev_fd != -1)
    {
        dup2(prev_fd, STDIN_FILENO);
        close(prev_fd);
    }

    // 🔹 Gestion du pipe en sortie
    if (cmd->next)
    {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
    }

    // 🔹 Redirections normales
    if (apply_redirs(cmd) == -1)
        exit(1);

    // 🔹 Builtins dans pipeline
    if (is_builtin(cmd->args[0]))
    {
        exec_builtin(cmd, sh);
        exit(sh->exit_status);
    }

    // 🔹 Commande externe
    exec_external(cmd, sh);
    exit(127);
}



void	pipeline_parent(t_cmd *cmd, int *prev_fd, int pipefd[2])
{
	// signal(SIGINT, SIG_IGN);
	// signal(SIGQUIT, SIG_IGN);
	if (*prev_fd != -1)
		close(*prev_fd);
	if (cmd->next)
	{
		close(pipefd[1]);
		*prev_fd = pipefd[0];
	}
}

void    pipeline_wait_last(pid_t last_pid, t_minishell *sh)
{
    int     status;
    pid_t   p;

    while ((p = waitpid(-1, &status, 0)) > 0)
    {
        if (p == last_pid)
        {
            if (WIFEXITED(status))
                sh->exit_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                sh->exit_status = 128 + WTERMSIG(status);
        }
    }
}

