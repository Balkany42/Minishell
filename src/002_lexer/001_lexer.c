/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   006_lexer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrager <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 22:04:42 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/16 21:32:04 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*lexer(char *line)
{
	if (!line || line[0] == '\0')
		return (NULL);
	return (lexer_loop(line));
}

t_token	*lexer_loop(char *line)
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

int	handle_operator(t_token **list, char *line, int i, t_tokentype type)
{
	char	*tmp;

	if (type == HEREDOC || type == APPEND)
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
