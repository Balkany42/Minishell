#include "../include/minishell.h"

int g_signal = 0;

void    handler_sigint(int sig)
{
    (void) sig;
    g_signal = SIGINT;
    rl_on_new_line();
    rl_replace_line("", 0);
    write(1, "\n", 1);
    rl_redisplay();
}

void    handler_sigquit(int sig)
{
    (void) sig;
    g_signal = SIGQUIT;
}

void init_signals()
{
    signal(SIGINT, handler_sigint);
    signal(SIGQUIT, handler_sigquit);
}