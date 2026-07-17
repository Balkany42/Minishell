#include "../include_bonus/minishell.h"

int has_unquoted_wildcard(char *s)
{
    int i = 0;
    int in_single = 0;
    int in_double = 0;

    while (s[i])
    {
        if (s[i] == '\'' && !in_double)
            in_single = !in_single;
        else if (s[i] == '"' && !in_single)
            in_double = !in_double;
        else if (s[i] == '*' && !in_single && !in_double)
            return (1);
        i++;
    }
    return (0);
}
void    wild_add_back(t_wildlist **lst, char *name)
{
    t_wildlist *new;
    t_wildlist *tmp;

    new = malloc(sizeof(t_wildlist));
    if (!new)
        return ;
    new->name = ft_strdup(name);
    new->next = NULL;
    if (!*lst)
    {
        *lst = new;
        return ;
    }
    tmp = *lst;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new;
}
t_wildlist   *wild_read_directory(void)
{
    DIR             *dir;
    struct dirent   *entry;
    t_wildlist      *lst;

    lst = NULL;
    dir = opendir(".");
    if (!dir)
        return (NULL);
    entry = readdir(dir);
    while (entry)
    {
        wild_add_back(&lst, entry->d_name);
        entry = readdir(dir);
    }
    closedir(dir);
    return (lst);
}
// char    *expand_wildcard(char *pattern)
// {
//     t_wildlist *files;
//     t_wildlist *tmp;

//     files = wild_read_directory();
//     tmp = files;

//     while(tmp)
//     {

//     }

//     // Étape 3 : filtrer selon le pattern
//     // Étape 4 : reconstruire la string ou la liste de tokens

//     return (pattern); // temporaire
// }

char    *expand_wildcard(char *pattern)
{
    t_wildlist   *files;
    t_wildlist   *tmp;
    char         *result;
    char         *joined;
    int           found;

    files = wild_read_directory();
    tmp = files;
    result = ft_strdup("");
    found = 0;

    while (tmp)
    {
        if (wild_match(pattern, tmp->name))
        {
            found = 1;
            joined = ft_strjoin(result, tmp->name);
            free(result);
            result = joined;

            joined = ft_strjoin(result, " ");
            free(result);
            result = joined;
        }
        tmp = tmp->next;
    }

    if (!found)
    {
        free(result);
        return (ft_strdup(pattern));
    }

    // enlever l’espace final
    if (result[0] && result[ft_strlen(result) - 1] == ' ')
        result[ft_strlen(result) - 1] = '\0';

    return (result);
}

int wild_match(char *pattern, char *name)
{
    int i = 0;
    int j = 0;

    while (pattern[i] && name[j])
    {
        if (pattern[i] == '*')
        {
            if (!pattern[i + 1])
                return (1);
            while (name[j] && name[j] != pattern[i + 1])
                j++;
            i++;
        }
        else if (pattern[i] == name[j])
        {
            i++;
            j++;
        }
        else
            return (0);
    }
    if (pattern[i] == '*' && !pattern[i + 1])
        return (1);
    return (pattern[i] == name[j]);
}
void    free_split(char **arr)
{
    int i;

    if (!arr)
        return ;
    i = 0;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}
