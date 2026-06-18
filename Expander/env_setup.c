/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_setup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrelli <melchior.grellier42@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 17:41:44 by megrelli          #+#    #+#             */
/*   Updated: 2026/06/18 19:48:08 by megrelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*expand_word(char *word, t_env *env)
{
	int	i;
	char	*new_str;

	i = 0;
	new_str = ft_strdup("");
	while (word[i])
	{
		if (word[i] == '$')
		{
			i++;
			while (word[i] <= )
		}
	}
	return (word);
}