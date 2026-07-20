/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   001_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrager <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 20:36:59 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/13 23:23:46 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_minishell	sh;

	(void)argc;
	(void)argv;
	init_minishell(&sh, envp);
	run_minishell_loop(&sh);
	cleanup_minishell(&sh);
	exit(sh.exit_status);
}
