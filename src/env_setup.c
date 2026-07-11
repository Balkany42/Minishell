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

char *expand_word(char *word, t_minishell *sh)
{
    int     i = 0;
    char    *result = ft_strdup("");
    char    *tmp;
    char    *var;
    char    *value;

    while (word[i])
    {
        // 🔥 1) Cas: \$ → $ littéral, pas d'expansion
        if (word[i] == '\\' && word[i + 1] == '$')
        {
            tmp = result;
            result = ft_strjoin(result, "$");
            free(tmp);
            i += 2;
            continue;
        }

        // 🔥 2) Cas: $ → expansion
        if (word[i] == '$')
        {
            i++;

            // $?
            if (word[i] == '?')
            {
                tmp = result;
                value = ft_itoa(sh->exit_status);
                result = ft_strjoin(result, value);
                free(tmp);
                free(value);
                i++;
                continue;
            }

            // $ suivi d'un caractère non valide → $ littéral
            if (!ft_isalpha(word[i]) && word[i] != '_')
            {
                tmp = result;
                result = ft_strjoin(result, "$");
                free(tmp);
                continue;
            }

            // Lire nom de variable
            int start = i;
            while (ft_isalnum(word[i]) || word[i] == '_' || word[i] == '*')
                i++;

            var = ft_substr(word, start, i - start);
            value = env_get(sh->env, var);

            tmp = result;
            result = ft_strjoin(result, value ? value : "");
            free(tmp);
            free(var);
            continue;
        }

        // 🔥 3) Cas: backslash général
        if (word[i] == '\\')
        {
            if (word[i + 1])
            {
                char buf[2] = {word[i + 1], 0};
                tmp = result;
                result = ft_strjoin(result, buf);
                free(tmp);
                i += 2;
            }
            else
            {
                char buf[2] = {'\\', 0};
                tmp = result;
                result = ft_strjoin(result, buf);
                free(tmp);
                i++;
            }
            continue;
        }

        // 🔥 4) Cas: caractère normal
        char buf[2] = {word[i], 0};
        tmp = result;
        result = ft_strjoin(result, buf);
        free(tmp);
        i++;
    }
    return result;
}

char *expand_token(t_token *tok, t_minishell *sh)
{
    if (tok->quote_type == SINGLE_QUOTE)
        return ft_strdup(tok->value);  // 🔥 littéral, aucune expansion

    // DOUBLE_QUOTE ou NO_QUOTE → expansion normale
    return expand_word(tok->value, sh);
}
