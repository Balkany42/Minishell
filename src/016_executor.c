/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   013_executor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrager <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 01:24:47 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/13 01:24:52 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	executor(t_cmd *cmds, t_minishell *sh)
{
	if (!cmds)
    {
        sh->exit_status = 1;   // erreur de parsing
        return;
    }
	if (!cmds->next)
		exec_single(cmds, sh);
	else
		exec_pipeline(cmds, sh);
}
