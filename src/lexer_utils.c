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
    char    *result = ft_strdup("");
    char    *tmp;
    char    *part;

    while (s[*i] &&
           s[*i] != ' ' &&
           s[*i] != '\t' &&
           is_operator(s, *i) == WORD)
    {
        if (s[*i] == '\'')
        {
            part = read_single_quote(s, i);
            if (!part)
                return (NULL);
        }
        else if (s[*i] == '"')
        {
            part = read_double_quote(s, i);
            if (!part)
                return (NULL);
        }
        else
        {
            int start = *i;
            while (s[*i] &&
                   s[*i] != ' ' &&
                   s[*i] != '\t' &&
                   s[*i] != '\'' &&
                   s[*i] != '"' &&
                   is_operator(s, *i) == WORD)
                (*i)++;
            part = ft_substr(s, start, *i - start);
        }

        tmp = result;
        result = ft_strjoin(result, part);
        free(tmp);
        free(part);
    }
    return (result);
}


char *read_single_quote(char *s, int *i)
{
    int start;
    char *content;

    (*i)++; // skip opening quote
    start = *i;

    while (s[*i] && s[*i] != '\'')
        (*i)++;

    if (!s[*i])
        return (NULL);

    content = ft_substr(s, start, *i - start);
    (*i)++; // skip closing quote
    return (content);
}

char *read_double_quote(char *s, int *i)
{
    int start;
    char *content;

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
void free_tokens(t_token *list)
{
    t_token *tmp;

    while (list)
    {
        tmp = list->next;
        free(list->value);
        free(list);
        list = tmp;
    }
}
// Fonction pour pourvoir print les tokens. Sert à debugger.

// void print_tokens(t_token *list)
// {
//     while (list)
//     {
//         printf("TYPE=%d | VALUE=\"%s\"\n", list->type, list->value);
//         list = list->next;
//     }
// }
