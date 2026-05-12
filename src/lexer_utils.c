#include "../include/minishell.h"

int skip_spaces(char *s, int i)
{
    while(s[i] == ' ' || s[i] == '\t')
        i++;
    return(i);
}

t_tokentype   is_operator(char *s, int i)
{
    if (s[i] == '|')
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
t_token *token_new(char *value, t_tokentype type)
{
    t_token *new;

    new = malloc(sizeof(t_token));
    if (!new)
        return (NULL);
    new->value = ft_strdup(value);
    new->type = type;
    new->next = NULL;
    return (new);
}

void add_token(t_token **list, char *value, t_tokentype type)
{
    t_token *new;
    t_token *tmp;

    new = token_new(value, type);
    if (!new)
        return ;
    if (*list == NULL)
    {
        *list = new;
        return ;
    }
    tmp = *list;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new;
}

char *read_word(char *s, int *i)
{
    int start = *i;

    while (s[*i] && s[*i] != ' ' && s[*i] != '\t'
        && is_operator(s, *i) == WORD)
    {
        (*i)++;
    }
    return (ft_substr(s, start, *i - start));
}