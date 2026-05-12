#include "../include/minishell.h"

char *ft_strdup(char *src)
{
    int i = 0;
    int *dest;

    while(src[i])
        i++;
    dest = malloc(i + 1);
    if(!dest)
        return(NULL);
    i = 0;
    while(src[i])
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return(dest);
}