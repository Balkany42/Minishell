/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   005_signals.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrager <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 21:48:38 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/13 23:21:49 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	g_signal = 0;

void	handler_sigint(int sig)
{
	(void)sig;
	g_signal = SIGINT;
	rl_on_new_line();
	rl_replace_line("", 0);
	write(1, "\n", 1);
	rl_redisplay();
}

void	init_signals(void)
{
	rl_catch_signals = 0;
	signal(SIGINT, handler_sigint);
	signal(SIGQUIT, SIG_IGN);
}
