/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   007_lexer_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrager <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 22:22:15 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/17 00:27:13 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_tokentype	is_operator(char *s, int i)
{
	if ((s[i] == '&' && s[i + 1] == '&')
		|| (s[i] == '|' && s[i + 1] == '|')
		|| (s[i] == ';' && s[i + 1] == ';')
		|| (s[i] == '|' && s[i + 1] == '&')
		|| s[i] == '&' || s[i] == ';'
		|| s[i] == '(' || s[i] == ')')
		return (INVALID);
	if (s[i] == '|')
		return (PIPE);
	if (s[i] == '<' && s[i + 1] == '<')
		return (HEREDOC);
	if (s[i] == '>' && s[i + 1] == '>')
		return (APPEND);
	if (s[i] == '<')
		return (REDIR_IN);
	if (s[i] == '>')
		return (REDIR_OUT);
	return (WORD);
}

t_token	*token_new(char *value, t_tokentype type)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = ft_strdup(value);
	new->type = type;
	new->next = NULL;
	new->quote_type = NO_QUOTE;
	return (new);
}

int	read_word(t_token **list, char *s, int *i)
{
	char	*buf;
	char	*part;
	t_quote	qtype;

	buf = ft_strdup("");
	qtype = NO_QUOTE;
	while (s[*i] && s[*i] != ' ' && s[*i] != '\t' && is_operator(s, *i) == WORD)
	{
		if (s[*i] == '\'')
		{
			if (!handle_single_quote(&buf, s, i, &qtype))
				return (0);
		}
		else if (s[*i] == '"')
		{
			if (!handle_double_quote(&buf, s, i, &qtype))
				return (0);
		}
		else
		{
			part = read_unquoted(s, i);
			buf = str_join_and_free(buf, part);
		}
	}
	return (add_token_with_quote(list, buf, WORD, qtype), free(buf), 1);
}

int	handle_single_quote(char **buf, char *s, int *i, t_quote *qtype)
{
	char	*part;

	*qtype = SINGLE_QUOTE;
	part = read_single_quote(s, i);
	if (!part)
	{
		free(*buf);
		return (0);
	}
	*buf = str_join_and_free(*buf, part);
	return (1);
}

int	handle_double_quote(char **buf, char *s, int *i, t_quote *qtype)
{
	char	*part;

	*qtype = DOUBLE_QUOTE;
	part = read_double_quote(s, i);
	if (!part)
	{
		free(*buf);
		return (0);
	}
	*buf = str_join_and_free(*buf, part);
	return (1);
}
