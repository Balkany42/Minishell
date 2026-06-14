#include "../include/minishell.h"

t_token *lexer(char *line)
{
    t_token *list = NULL;
    int     i = 0;
    t_tokentype type;
    char    *word;

    while (line[i])
    {
        i = skip_spaces(line, i);
        if (!line[i])
            break;
        type = is_operator(line, i);
        if (type != WORD)
        {
            if (type == HEREDOC || type == APPEND)
                add_token(&list, ft_substr(line, i, 2), type), i += 2;
            else
                add_token(&list, ft_substr(line, i, 1), type), i += 1;
        }
        else
        {
            word = read_word(line, &i);
            if (!word)
            {
                free_tokens(list);
                return (NULL);
            }
            add_token(&list, word, WORD);
            free(word);
        }
    }
    return (list);
}
