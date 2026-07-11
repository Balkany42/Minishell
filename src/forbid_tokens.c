#include "../include/minishell.h"

int check_syntax_errors(t_token *t)
{
    t_token *n;

    if (!t)
        return 0;
    if (t->type == PIPE || t->type == REDIR_IN
        || t->type == REDIR_OUT || t->type == APPEND || t->type == INVALID)
        return 1;
    while (t)
    {
        n = t->next;
        if (t->type == INVALID)
            return 1;
        if (!n && (t->type == PIPE || t->type == REDIR_IN
            || t->type == REDIR_OUT || t->type == APPEND))
            return 1;
        if (n)
        {
            if (t->type == PIPE && n->type == PIPE)
                return 1;
            if (t->type == PIPE && (n->type == REDIR_IN
                || n->type == REDIR_OUT || n->type == APPEND))
                return 1;
            if ((t->type == REDIR_IN || t->type == REDIR_OUT
                || t->type == APPEND) && n->type == PIPE)
                return 1;
            if ((t->type == REDIR_IN || t->type == REDIR_OUT
                || t->type == APPEND)
                && (n->type == REDIR_IN || n->type == REDIR_OUT
                || n->type == APPEND))
                return 1;
            if ((t->type == REDIR_IN || t->type == REDIR_OUT
                || t->type == APPEND)
                && n->type != WORD)
                return 1;
        }
        t = t->next;
    }
    return 0;
}
