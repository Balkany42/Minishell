/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrelli <melchior.grellier42@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 22:22:15 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/21 01:53:19 by megrelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell_bonus.h"

static int	handle_single_quote(char **buf, char *s, int *i, t_quote *qtype)
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

static int	handle_double_quote(char **buf, char *s, int *i, t_quote *qtype)
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

static int	read_word_loop(char **buf, char *s, int *i, t_quote *qtype)
{
	char	*part;

	while (s[*i] && s[*i] != ' ' && s[*i] != '\t' && s[*i] != '('
		&& s[*i] != ')' && s[*i] != '|' && s[*i] != '&'
		&& s[*i] != '<' && s[*i] != '>')
	{
		if (s[*i] == '\'')
		{
			if (!handle_single_quote(buf, s, i, qtype))
				return (0);
		}
		else if (s[*i] == '"')
		{
			if (!handle_double_quote(buf, s, i, qtype))
				return (0);
		}
		else
		{
			part = read_unquoted(s, i);
			*buf = str_join_and_free(*buf, part);
		}
	}
	return (1);
}

int	read_word(t_token **list, char *s, int *i)
{
	char	*buf;
	t_quote	qtype;

	buf = ft_strdup("");
	qtype = NO_QUOTE;
	if (!read_word_loop(&buf, s, i, &qtype))
		return (free(buf), 0);
	return (add_token_with_quote(list, buf, WORD, qtype), free(buf), 1);
}
