#include "../include/minishell.h"

int g_signal = 0;

void handler_sigint(int sig)
{
    (void)sig;
    g_signal = SIGINT;
    rl_on_new_line();
    rl_replace_line("", 0);
    write(1, "\n", 1);
    rl_redisplay();
}

void init_signals(void)
{
    signal(SIGINT, handler_sigint);
    signal(SIGQUIT, SIG_IGN);
}
