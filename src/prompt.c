#include "../include/minishell.h"

char *readprompt()
{
    char *line;
    const char *prompt = "minishell$ ";
    line = readline(prompt);
    return(line);
}

int is_blank_line(char *s)
{
    int i = 0;

    while (s[i])
    {
        if (s[i] != ' ' && s[i] != '\t')
            return 0; // contient un vrai caractère → OK
        i++;
    }
    return 1; // que des espaces/tabulations → ne pas ajouter
}
