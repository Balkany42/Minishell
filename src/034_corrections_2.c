/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   034_corrections_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrager <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 04:30:56 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/16 04:34:42 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// void	prepare_heredocs(t_cmd *cmds, t_minishell *sh)
// {
// 	t_cmd	*cmd = cmds;
// 	t_redir	*r;

// 	while (cmd)
// 	{
// 		r = cmd->redirs;
// 		while (r)
// 		{
// 			if (r->type == HEREDOC)
// 			{
// 				char *tmp = generate_heredoc_tmp_name();
// 				if (!tmp)
// 				{
// 					sh->exit_status = 1;
// 					return ;
// 				}
// 				pid_t pid = fork();
// 				if (pid == 0)
// 				{
// 					rl_catch_signals = 1;
// 					signal(SIGINT, SIG_DFL);
// 					signal(SIGQUIT, SIG_IGN);
// 					if (write_heredoc_to(tmp, r->file) < 0)
// 						exit(1);
// 					exit(0);
// 				}
// 				int status;
// 				signal(SIGINT, SIG_IGN); 
// 				waitpid(pid, &status, 0);
// 				signal(SIGINT, handler_sigint);
// 				if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
// 				{
// 					sh->exit_status = 130;
// 					sh->heredoc_interrupted = 1;
// 					free(tmp);
// 					return ;
// 				}
// 				if (WEXITSTATUS(status) != 0)
// 				{
// 					sh->exit_status = 1;
// 					free(tmp);
// 					return ;
// 				}
// 				free(r->file);
// 				r->file = tmp;
// 				r->type = REDIR_IN;
// 				}
// 			r = r->next;
// 		}
// 		cmd = cmd->next;
// 	}
// }
