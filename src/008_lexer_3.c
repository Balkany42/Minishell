/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   007_lexer_3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrager <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 22:57:18 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/12 23:02:42 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	add_token_with_quote(t_token **list,
char *value, t_tokentype type, t_quote q)
{
	t_token	*new;
	t_token	*tmp;

	new = malloc(sizeof(t_token));
	if (!new)
		return ;
	new->value = ft_strdup(value);
	new->type = type;
	new->quote_type = q;
	new->next = NULL;
	if (!*list)
	{
		*list = new;
		return ;
	}
	tmp = *list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

char	*read_single_quote(char *s, int *i)
{
	int		start;
	char	*content;

	(*i)++;
	start = *i;
	while (s[*i] && s[*i] != '\'')
		(*i)++;
	if (!s[*i])
		return (NULL);
	content = ft_substr(s, start, *i - start);
	(*i)++;
	return (content);
}

char	*read_double_quote(char *s, int *i)
{
	int		start;
	char	*content;

	(*i)++;
	start = *i;
	while (s[*i] && s[*i] != '"')
		(*i)++;
	if (!s[*i])
		return (NULL);
	content = ft_substr(s, start, *i - start);
	(*i)++;
	return (content);
}

void	free_tokens(t_token *list)
{
	t_token	*tmp;

	while (list)
	{
		tmp = list->next;
		free(list->value);
		free(list);
		list = tmp;
	}
}

char	*read_unquoted(char *s, int *i)
{
	int	start;

	start = *i;
	while (s[*i] && s[*i] != ' ' && s[*i] != '\t'
		&& s[*i] != '\'' && s[*i] != '"'
		&& is_operator(s, *i) == WORD)
		(*i)++;
	return (ft_substr(s, start, *i - start));
}
