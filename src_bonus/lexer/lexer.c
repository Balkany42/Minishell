/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrelli <melchior.grellier42@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 22:04:42 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/21 01:53:10 by megrelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell_bonus.h"

static int	handle_operator(t_token **list, char *line, int i, t_tokentype type)
{
	char	*tmp;

	if (type == HEREDOC || type == APPEND || type == AND_IF || type == OR_IF)
	{
		tmp = ft_substr(line, i, 2);
		add_token_with_quote(list, tmp, type, NO_QUOTE);
		free(tmp);
		return (i + 2);
	}
	tmp = ft_substr(line, i, 1);
	add_token_with_quote(list, tmp, type, NO_QUOTE);
	free(tmp);
	return (i + 1);
}

static t_token	*lexer_loop(char *line)
{
	t_token		*list;
	int			i;
	t_tokentype	type;

	list = NULL;
	i = 0;
	while (line[i])
	{
		i = skip_spaces(line, i);
		if (!line[i])
			break ;
		type = is_operator(line, i);
		if (type != WORD)
			i = handle_operator(&list, line, i, type);
		else
		{
			if (!read_word(&list, line, &i))
			{
				free_tokens(list);
				return (NULL);
			}
		}
	}
	return (list);
}

t_token	*lexer(char *line)
{
	if (!line || line[0] == '\0')
		return (NULL);
	return (lexer_loop(line));
}

t_tokentype	is_operator(char *s, int i)
{
	if (s[i] == '&' && s[i + 1] == '&')
		return (AND_IF);
	if (s[i] == '|' && s[i + 1] == '|')
		return (OR_IF);
	if (s[i] == '(')
		return (LPAREN);
	if (s[i] == ')')
		return (RPAREN);
	if ((s[i] == ';' && s[i + 1] == ';') || (s[i] == '|' && s[i + 1] == '&')
		|| (s[i] == '&' && s[i + 1] == '|') || s[i] == '&' || s[i] == ';')
		return (INVALID);
	if (s[i] == '|' && s[i + 1] != '|')
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
