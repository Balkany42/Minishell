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

int strcmp_bash(const char *a, const char *b)
{
    int i = 0;

    while (a[i] && b[i])
    {
        char ca = a[i];
        char cb = b[i];

        // règle bash : les minuscules passent avant les majuscules
        if (ca != cb)
        {
            // si les deux sont des lettres
            if (ft_isalpha(ca) && ft_isalpha(cb))
            {
                // comparer en ignorant la casse
                char la = ft_tolower(ca);
                char lb = ft_tolower(cb);

                if (la != lb)
                    return (la - lb);

                // si même lettre mais casse différente :
                // minuscule < majuscule
                return (ca - cb);
            }

            // sinon tri ASCII normal
            return (ca - cb);
        }

        i++;
    }

    return (a[i] - b[i]);
}


void bubble_sort(char **tab)
{
    int     i;
    int     j;
    char    *tmp;

    for (i = 0; tab[i]; i++)
    {
        for (j = i + 1; tab[j]; j++)
        {
            if (strcmp_bash(tab[i], tab[j]) > 0)
            {
                tmp = tab[i];
                tab[i] = tab[j];
                tab[j] = tmp;
            }
        }
    }
}


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
        // if (tmp->name[0] == '.' && pattern[0] != '.')
        // {
        //     tmp = tmp->next;
        //     continue;
        // }
        if (wild_match(pattern, tmp->name))
        {
            if (tmp->name[0] == '.' && pattern[0] != '.')
            {
                tmp = tmp->next;
                continue;
            }
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

    wild_clear(&files);

    if (!found)
    {
        free(result);
        return (ft_strdup(pattern));
    }

    // enlever l’espace final
    if (result[0] && result[ft_strlen(result) - 1] == ' ')
        result[ft_strlen(result) - 1] = '\0';

    // SPLIT → TRIER → REJOIN
    char **tab = ft_split(result, ' ');
    free(result);

    bubble_sort(tab);

    // Rejoin propre
    result = ft_strdup("");
    for (int i = 0; tab[i]; i++)
    {
        joined = ft_strjoin(result, tab[i]);
        free(result);
        result = joined;

        if (tab[i + 1])
        {
            joined = ft_strjoin(result, " ");
            free(result);
            result = joined;
        }
    }

    free_split(tab);
    return result;
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
void    wild_clear(t_wildlist **lst)
{
    t_wildlist *cur;
    t_wildlist *next;

    cur = *lst;
    while (cur)
    {
        next = cur->next;
        free(cur->name);
        free(cur);
        cur = next;
    }
    *lst = NULL;
}
