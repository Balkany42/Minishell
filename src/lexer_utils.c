#include "../include/minishell.h"

int skip_spaces(char *s, int i)
{
    while(s[i] == ' ' || s[i] == '\t')
        i++;
    return(i);
}

t_tokentype is_operator(char *s, int i)
{
    // 🔥 Tous les tokens interdits dans minishell
    if (s[i] == '&' && s[i + 1] == '&')
        return (INVALID);      // &&
    if (s[i] == '|' && s[i + 1] == '|')
        return (INVALID);      // ||
    if (s[i] == ';' && s[i + 1] == ';')
        return (INVALID);      // ;;
    if (s[i] == '|' && s[i + 1] == '&')
        return (INVALID);      // |&
    if (s[i] == '&')
        return (INVALID);      // &
    if (s[i] == ';')
        return (INVALID);      // ;
    if (s[i] == '(' || s[i] == ')')
    return (INVALID);

    // 🔥 Opérateurs valides
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

void read_word(t_token **list, char *s, int *i)
{
    while (s[*i] && s[*i] != ' ' && s[*i] != '\t' && is_operator(s, *i) == WORD)
    {
        if (s[*i] == '\'')
        {
            char *part = read_single_quote(s, i);
            add_token_with_quote(list, part, WORD, SINGLE_QUOTE);
            free(part);
        }
        else if (s[*i] == '"')
        {
            char *part = read_double_quote(s, i);
            add_token_with_quote(list, part, WORD, DOUBLE_QUOTE);
            free(part);
        }
        else
        {
            int start = *i;
            while (s[*i] && s[*i] != ' ' && s[*i] != '\t'
                   && s[*i] != '\'' && s[*i] != '"'
                   && is_operator(s, *i) == WORD)
                (*i)++;

            char *part = ft_substr(s, start, *i - start);
            add_token_with_quote(list, part, WORD, NO_QUOTE);
            free(part);
        }
    }
}


void add_token_with_quote(t_token **list, char *value, t_tokentype type, t_quote q)
{
    t_token *new = malloc(sizeof(t_token));
    if (!new)
        return;

    new->value = ft_strdup(value);
    new->type = type;
    new->quote_type = q;
    new->next = NULL;

    if (!*list)
    {
        *list = new;
        return;
    }

    t_token *tmp = *list;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new;
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