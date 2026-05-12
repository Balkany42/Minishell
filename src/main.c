#include "../include/minishell.h"

int main(int argc, char **argv)
{
    char *line;

    init_signals();
    while(1)
    {
        line = readprompt();
        if(!line)
        {
            write(1, "exit\n", 5);
            break;
        }
        if(line[0] != '\0')
            add_history(line);
        free(line);
    }
}