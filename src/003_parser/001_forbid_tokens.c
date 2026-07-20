/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   008_forbid_tokens.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrager <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 23:03:27 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/13 00:07:39 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_syntax_errors(t_token *t)
{
	t_token	*n;

	if (check_first_token(t))
		return (1);
	while (t)
	{
		n = t->next;
		if (check_invalid_token(t))
			return (1);
		if (check_end_of_line(t, n))
			return (1);
		if (n && check_between_tokens(t, n))
			return (1);
		t = t->next;
	}
	return (0);
}

int	check_first_token(t_token *t)
{
	if (!t)
		return (0);
	if (t->type == PIPE || t->type == REDIR_IN
		|| t->type == REDIR_OUT || t->type == APPEND
		|| t->type == INVALID)
		return (1);
	return (0);
}

int	check_invalid_token(t_token *t)
{
	if (t->type == INVALID)
		return (1);
	return (0);
}

int	check_end_of_line(t_token *t, t_token *n)
{
	if (!n && (t->type == PIPE || t->type == REDIR_IN
			|| t->type == REDIR_OUT || t->type == APPEND))
		return (1);
	return (0);
}

int	check_between_tokens(t_token *t, t_token *n)
{
	if (t->type == PIPE && n->type == PIPE)
		return (1);
	if (t->type == PIPE && (n->type == REDIR_IN
			|| n->type == REDIR_OUT || n->type == APPEND))
		return (1);
	if ((t->type == REDIR_IN || t->type == REDIR_OUT
			|| t->type == APPEND) && n->type == PIPE)
		return (1);
	if ((t->type == REDIR_IN || t->type == REDIR_OUT
			|| t->type == APPEND)
		&& (n->type == REDIR_IN || n->type == REDIR_OUT
			|| n->type == APPEND))
		return (1);
	if ((t->type == REDIR_IN || t->type == REDIR_OUT
			|| t->type == APPEND) && n->type != WORD)
		return (1);
	return (0);
}
