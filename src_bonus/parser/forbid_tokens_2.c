/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forbid_tokens_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrelli <melchior.grellier42@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 05:05:19 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/21 01:53:34 by megrelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell_bonus.h"

static int	check_paren_local(t_token *t, t_token *prev, int *depth)
{
	if (t->type == LPAREN)
	{
		if (prev && prev->type == RPAREN)
			return (1);
		(*depth)++;
		if (t->next && t->next->type == RPAREN)
			return (1);
	}
	else if (t->type == RPAREN)
	{
		(*depth)--;
		if (*depth < 0)
			return (1);
		if (t->next && t->next->type == LPAREN)
			return (1);
	}
	return (0);
}

int	check_pipe_and_redir_relations(t_token *t, t_token *n)
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

int	check_logic_and_paren_relations(t_token *t, t_token *n)
{
	if ((t->type == PIPE || t->type == REDIR_IN || t->type == REDIR_OUT
			|| t->type == APPEND) && (n->type == AND_IF || n->type == OR_IF))
		return (1);
	if ((t->type == AND_IF || t->type == OR_IF) && (n->type == PIPE
			|| n->type == REDIR_IN || n->type == REDIR_OUT
			|| n->type == APPEND))
		return (1);
	if ((t->type == AND_IF || t->type == OR_IF)
		&& (n->type == AND_IF || n->type == OR_IF))
		return (1);
	if (t->type == LPAREN && (n->type == AND_IF || n->type == OR_IF))
		return (1);
	if ((t->type == AND_IF || t->type == OR_IF) && n->type == RPAREN)
		return (1);
	if ((n->type == AND_IF || n->type == OR_IF)
		&& !(t->type == WORD || t->type == RPAREN))
		return (1);
	if ((t->type == AND_IF || t->type == OR_IF)
		&& !(n->type == WORD || n->type == LPAREN))
		return (1);
	if (t->type == LPAREN && n->type == RPAREN)
		return (1);
	if (t->type == RPAREN && n->type == LPAREN)
		return (1);
	return (0);
}

int	check_parentheses_balance(t_token *t)
{
	int	count;

	count = 0;
	while (t)
	{
		if (t->type == LPAREN)
			count++;
		else if (t->type == RPAREN)
			count--;
		if (count < 0)
			return (1);
		t = t->next;
	}
	return (count != 0);
}

int	check_parentheses_order(t_token *t)
{
	int		depth;
	t_token	*prev;

	depth = 0;
	prev = NULL;
	while (t)
	{
		if (check_paren_local(t, prev, &depth))
			return (1);
		prev = t;
		t = t->next;
	}
	if (depth != 0)
		return (1);
	return (0);
}
