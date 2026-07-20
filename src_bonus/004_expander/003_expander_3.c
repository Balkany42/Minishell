/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   C.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrager <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 13:47:29 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/13 13:52:34 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include_bonus/minishell.h"

char	*handle_backslash(char *result, char *word, int *i)
{
	char	buf[2];
	char	*tmp;

	if (word[*i + 1])
	{
		buf[0] = word[*i + 1];
		buf[1] = 0;
		*i += 2;
	}
	else
	{
		buf[0] = '\\';
		buf[1] = 0;
		(*i)++;
	}
	tmp = result;
	result = ft_strjoin(result, buf);
	free(tmp);
	return (result);
}

char	*append_char(char *result, char c)
{
	char	buf[2];
	char	*tmp;

	buf[0] = c;
	buf[1] = 0;
	tmp = result;
	result = ft_strjoin(result, buf);
	free(tmp);
	return (result);
}

char	*handle_var_expansion(char *result,
	char *word, int *i, t_minishell *sh)
{
	char	*var;
	char	*value;
	char	*tmp;

	var = read_var_name(word, i);
	value = env_get(sh->env, var);
	tmp = result;
	if (value)
		result = ft_strjoin(result, value);
	else
		result = ft_strjoin(result, "");
	free(tmp);
	free(var);
	return (result);
}

char	*read_var_name(char *word, int *i)
{
	int	start;

	start = *i;
	while (ft_isalnum(word[*i]) || word[*i] == '_')
		(*i)++;
	return (ft_substr(word, start, *i - start));
}
