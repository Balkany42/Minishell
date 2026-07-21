/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrelli <melchior.grellier42@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 20:36:59 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/20 17:38:31 by megrelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell_bonus.h"

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
