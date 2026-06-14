#include "../include/minishell.h"

char *readprompt()
{
    char *line;
    const char *prompt = "minishell$ ";
    line = readline(prompt);
    return(line);
}