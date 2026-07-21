/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrelli <melchior.grellier42@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 17:41:44 by megrelli          #+#    #+#             */
/*   Updated: 2026/07/21 01:56:04 by megrelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell_bonus.h"

static char	*handle_invalid_var(char *result)
{
	char	*tmp;

	tmp = result;
	result = ft_strjoin(result, "$");
	free(tmp);
	return (result);
}

static char	*handle_exit_status(char *result, t_minishell *sh, int *i)
{
	char	*tmp;
	char	*value;

	tmp = result;
	value = ft_itoa(sh->exit_status);
	result = ft_strjoin(result, value);
	free(tmp);
	free(value);
	(*i) += 1;
	return (result);
}

static char	*handle_escaped_dollar(char *result, int *i)
{
	char	*tmp;

	tmp = result;
	result = ft_strjoin(result, "$");
	free(tmp);
	*i += 2;
	return (result);
}

static char	*process_expansion(char *result, char *word, int *i,
		t_minishell *sh)
{
	if (word[*i] == '\\' && word[*i + 1] == '$')
		return (handle_escaped_dollar(result, i));
	if (word[*i] == '$')
	{
		(*i)++;
		if (word[*i] == '?')
			return (handle_exit_status(result, sh, i));
		if (!ft_isalpha(word[*i]) && word[*i] != '_')
			return (handle_invalid_var(result));
		return (handle_var_expansion(result, word, i, sh));
	}
	if (word[*i] == '\\')
		return (handle_backslash(result, word, i));
	return (append_char(result, word[(*i)++]));
}

char	*expand_word(char *word, t_minishell *sh)
{
	int		i;
	char	*result;

	i = 0;
	result = ft_strdup("");
	while (word[i])
		result = process_expansion(result, word, &i, sh);
	return (result);
}
