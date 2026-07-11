#include "../include/minishell.h"

t_token *lexer(char *line)
{
    t_token *list = NULL;
    int     i = 0;
    t_tokentype type;

    if (!line || line[0] == '\0')
        return NULL;

    while (line[i])
    {
        i = skip_spaces(line, i);
        if (!line[i])
            break;

        type = is_operator(line, i);

        if (type != WORD)
        {
            char *tmp;

            if (type == HEREDOC || type == APPEND)
            {
                tmp = ft_substr(line, i, 2);
                add_token_with_quote(&list, tmp, type, NO_QUOTE);
                free(tmp);
                i += 2;
            }
            else
            {
                tmp = ft_substr(line, i, 1);
                add_token_with_quote(&list, tmp, type, NO_QUOTE);
                free(tmp);
                i += 1;
            }
        }
        else
        {
            read_word(&list, line, &i);
        }
    }
    return list;
}
