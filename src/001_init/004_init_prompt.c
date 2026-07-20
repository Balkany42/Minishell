/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   003_prompt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrager <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 21:45:29 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/12 21:47:54 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*readprompt(void)
{
	char		*line;
	const char	*prompt = "minishell$ ";

	line = readline(prompt);
	return (line);
}

int	is_blank_line(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] != ' ' && s[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}
